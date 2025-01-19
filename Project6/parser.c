#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "hack_assembler.h"

/*
 *This function is for the first run through of the asm file
 *it takes all the labels and adds it to the symbol table
 */
void parse_symbol(char line[]) {
    static int valid_line = 0; //holds what valid line we are on (value for the label)
    char symbol[SYM_LEN]; //holds the symbol
    int i = 0;

    //skips white space
    while (isspace(line[i])) {
        if (line[i] == '\n' || line[i] == '\0') {//exits if blank line
            return;
        }
        i++;
    }
    //if line is a comment, exit
    if (line[i] == '/') {
        return;
    }
    /*
     *Valid lines of code that are not blank and not comments only have a few possible starts
     *which are tested for below. We need to keep track of how many valid instruction lines we have gone through so
     *the right value is put with the label
     */
    if (line[i] == 'D' || line[i] == 'A' || line[i] == 'M' || line[i] == '@'
        || line[i] == '0' || line[i] == '(' ) {
        /*if the line starts with '(', it is a label. We want to extract it and add it to the table
        *however as it is not a valid instruction line, valid_line should only increase if the line
        *does not start with '('
        */
        if (line[i] == '(') { //extracting symbol
            i++;
            int j;
            for (j=0; line[i] != ')' && j < SYM_LEN-1; j++,i++) {
                symbol[j] = line[i];
            }
            symbol[j] = '\0'; //terminating symbol
            sym_add(symbol, valid_line); //adding to table
             return;
        }
        //if it is a valid start and not a label, increase valid_line
        valid_line++;
    }

}


void parse_variable(char line[]) {
    static int new_address = 16; //variables that are not predefined start and address 16
    int i = 0;
    char symbol[SYM_LEN];

    //skips white space
    while (isspace(line[i])) {
        if (line[i] == '\n' || line[i] == '\0') {
            return;
        }
        i++;
    }

    if (line[i] == '@') {
        i++;
        if(line[i] >= '0' && line[i] <= '9') { //if A instruction has a number, exit, as it is not a variable
            return;
        }
        int j;
        for (j = 0; j<SYM_LEN -1 && !(isspace(line[i]) && line[i] != '/'); j++,i++ ) { // copy the variable name
            symbol[j] = line[i];
        }
        symbol[j] = '\0';

       if(sym_add(symbol, new_address)) { //sym_add returns 1 if the variable is new

           new_address++; //increase new_address for next variable

       }
        return;
    }
    return;

}
void parse_instruction(char line[], FILE* outfile) {
    char dest_str[4];
    dest_str[0] = '\0';
    char comp_str[4];
    comp_str[0] = '\0';
    char jmp_str[4];
    jmp_str[0] = '\0';

    char instruction[MAX_LINE];
    int i = 0;

    //skips white space
    while (isspace(line[i])) {
        if (line[i] == '\n' || line[i] == '\0') {
            return;
        }
        i++;
    }
    if (line[i] == '/' || line[i] == '(') {
        return;
    }
    //copying instruction
    int j;
    for (j = 0;!(isspace(line[i])) && line[i] != '/' && line[i] != '\0';j++, i++) {
        instruction[j] = line[i];
    }
    instruction[j] = '\0';

    i=0;
    if (instruction[0] == '@') {//if addressing instruction, handle with cooresponding helper function
        a_instr(instruction, outfile);
        return;
    }
    else {
        /*
         *The destination, or where the output value will be stored is only specified if the
         *instruction has a '='. If it does not, the destination string will just be null
         */
       if(contains_char(instruction, '=')) {
           for(; instruction[i] != '='; i++) {
               dest_str[i] = instruction[i];

           }

           dest_str[i] = '\0';

           i++;
       }
        /*
         *Here, the computation is copied into a new string. If the instruction is not an A instruciton, then it
         *must have a computation portion, so there is no requirement to check
         */
        j=0;
        while(!isspace(instruction[i]) && instruction[i] != '/' && instruction[i] != '\0'
            && instruction[i] != ';') {
            comp_str[j] = instruction[i];
            i++;
            j++;

        }
        comp_str[j] = '\0';

    /*
     *There is only a jump if there is a semicolon in the instruction, so that is a condition we must check for
     *if not, the jump bits are null
     */
        if(contains_char(instruction, ';')) {
            i++;
            j = 0;
            //copy jmp into jmp_str
            for (; !isspace(instruction[i]) && instruction[j] != '/' && instruction[i] != '\0'; j++, i++) {
                jmp_str[j] = instruction[i];
            }
            jmp_str[j] = '\0';
        }
        /*
         *All C instructions start with 111, so regardless of the nature of the C instruction
         *it must start with this
         *Then, we go in order for the order of the bits: 111|computation|destination|jump
         *Each of these functions takes the corresponding string with the relevant sub-instruction
         *and prints to the file the corresponding binary string
         */
        fputc('1', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
        comp_instr(comp_str, outfile);
        dest_instr(dest_str, outfile);
        jmp_instr(jmp_str, outfile);
        fputc('\n', outfile);

    }





}
