#include <stdio.h>

#include "hack_assembler.h"


int main(int argc, char *argv[]) {

    //initialize symbol table
    table_initialize();

    //holds each line from asm file
    char line[MAX_LINE];

    FILE *asmfile = fopen(argv[1], "r");//assembly file

    FILE *hackfile = fopen(argv[2], "w"); //binary file

    if (asmfile == NULL) {
        printf("Error opening file %s\n", argv[1]);
    }

    /* First run through of the file. This run through only adds labels marked by
     * (LABEL_HERE) and adds it to the table. This is because if @LABEL_HERE is encountered
     * it could be a label or variable. If it is a label, its line number must be put as the value
    */
    while(fgets(line, MAX_LINE, asmfile)!= NULL) {
            parse_symbol(line);
        }

    //start at the beginning of the file
    rewind(asmfile);

    /*this run through adds new symbols to the table, meaning only the variables, not labels. The value assigned
     *starts at 16, this is where the data for the value is held
     */
    while(fgets(line, MAX_LINE, asmfile)!= NULL) {
        parse_variable(line);
    }

    //rewind to beginning
    rewind(asmfile);

    /* This run through is the final one, it takes each line
     * and converts it to a 16 bit binary string containing the instruction described
     * in the assembly file
     */
    while(fgets(line, MAX_LINE, asmfile)!= NULL) {
        parse_instruction(line, hackfile);

    }

    return 0;
}






