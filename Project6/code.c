#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "hack_assembler.h"




//helper function, checks if a character is in a string
int contains_char(char line[], char character) { //add line size
    int i;
    for (i = 0; line[i] != '\0'; i++) {
        if (line[i] == character) {
            return 1;
        }
    }
    return 0;
}

//prints binary number representation of a number, used for A instructions
void print_bin(int num, FILE* outfile) {
    for (int i = 14; i >= 0; i--) {
        // Extract the bit at position `i`
        fprintf(outfile, "%d", (num >> i) & 1);
    }
    fprintf(outfile, "\n");

}

//this function creates the binary string for an A instruction
void a_instr(char instruction[], FILE* outfile) {
    int address;
    fputc('0', outfile); //A instructions always start with a 0
    int i;

    //checking if the instruction is a symbol or a number
    if (isdigit(instruction[1]) ) {
        //if it is a number, print the binary representation
        char num[7];

        for (i=1; instruction[i] != '\0'; i++) {
            num[i-1] = instruction[i];
        }
        num[i-1] = '\0';
        sscanf(num, "%d", &address);
        print_bin(address, outfile);
    }
    else {
        //otherwise print associated value stored in symbol table
        char symbol[SYM_LEN];
        for(i = 1; instruction[i] != '\0'; i++ ) {
            symbol[i-1] = instruction[i];
        }
        symbol[i-1] = '\0';
        print_bin(sym_get(symbol), outfile);
    }

}
//this function converts the destination portion of a C instruction
//to the corresponding binary substring
void dest_instr(char dest[], FILE* outfile) {
    if(dest[0] == '\0') {
        fputc('0', outfile);
        fputc('0', outfile);
        fputc('0', outfile);
        return;

    }
     if (contains_char(dest, 'A')) {
        fputc('1', outfile);
    }
    else {
        fputc('0', outfile);
    }

    if (contains_char(dest, 'D')) {
        fputc('1', outfile);
    }
    else {
        fputc('0', outfile);
    }
    if (contains_char(dest, 'M')) {
        fputc('1', outfile);
    }
    else {
        fputc('0', outfile);
    }
}
//this function converts the jump portion of a C instruction
//to the corresponding binary substring
void jmp_instr(char jump[], FILE* outfile) {
    if(jump[0] == '\0') {
        fputc('0', outfile);
        fputc('0', outfile);
        fputc('0', outfile);

    }
    else if (strcmp(jump, "JMP") == 0) {

        fputc('1', outfile);
        fputc('1', outfile);
        fputc('1', outfile);

    }
    else if (strcmp(jump, "JLE") == 0) {

        fputc('1', outfile);
        fputc('1', outfile);
        fputc('0', outfile);
    }
    else if (strcmp(jump, "JNE") == 0) {

        fputc('1', outfile);
        fputc('0', outfile);
        fputc('1', outfile);
    }
    else if (strcmp(jump, "JLT") == 0) {

        fputc('1', outfile);
        fputc('0', outfile);
        fputc('0', outfile);
    }
    else if (strcmp(jump, "JGE") == 0) {

        fputc('0', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
    }
    else if (strcmp(jump, "JEQ") == 0) {
        fputc('0', outfile);
        fputc('1', outfile);
        fputc('0', outfile);

    }
    else if (strcmp(jump, "JGT") == 0) {

        fputc('0', outfile);
        fputc('0', outfile);
        fputc('1', outfile);
    }
}
//this function converts the computation portion of a C instruction
//to the corresponding binary substring
void comp_instr(char comp[], FILE* outfile) {

    //this is the 'a bit'
    //determines if the computation uses the M register or the A register
    if (contains_char(comp, 'M')) {
        fputc('1', outfile);
    }
    else {
        fputc('0', outfile);
    }
    if(strcmp(comp, "0") == 0) {
        fputc('1', outfile);
        fputc('0', outfile);
        fputc('1', outfile);
        fputc('0', outfile);
        fputc('1', outfile);
        fputc('0', outfile);
    }
    else if(strcmp(comp, "1") == 0) {

        fputc('1', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
    }
    else if(strcmp(comp, "-1") == 0) {

        fputc('1', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
        fputc('0', outfile);
        fputc('1', outfile);
        fputc('0', outfile);
    }
    else if(strcmp(comp, "D") == 0) {

        fputc('0', outfile);
        fputc('0', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
        fputc('0', outfile);
        fputc('0', outfile);
    }
    else if(strcmp(comp, "M") == 0 || strcmp(comp, "A") == 0) {

        fputc('1', outfile);
        fputc('1', outfile);
        fputc('0', outfile);
        fputc('0', outfile);
        fputc('0', outfile);
        fputc('0', outfile);
    }
    else if(strcmp(comp, "!D") == 0) {

        fputc('1', outfile);
        fputc('0', outfile);
        fputc('1', outfile);
        fputc('0', outfile);
        fputc('1', outfile);
        fputc('1', outfile);

    }
    else if(strcmp(comp, "!M") == 0 || strcmp(comp, "!A") == 0) {

        fputc('1', outfile);
        fputc('1', outfile);
        fputc('0', outfile);
        fputc('0', outfile);
        fputc('0', outfile);
        fputc('1', outfile);

    }
    else if(strcmp(comp, "-D") == 0) {

        fputc('0', outfile);
        fputc('0', outfile);
        fputc('0', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
        fputc('1', outfile);


    }
    else if(strcmp(comp, "-A") == 0 || strcmp(comp, "-M") == 0) {

        fputc('1', outfile);
        fputc('1', outfile);
        fputc('0', outfile);
        fputc('0', outfile);
        fputc('1', outfile);
        fputc('1', outfile);


    }
    else if(strcmp(comp, "D+1") == 0 ) {
        fputc('0', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
    }
    else if(strcmp(comp, "A+1") == 0 || strcmp(comp, "M+1") == 0 ) {

        fputc('1', outfile);
        fputc('1', outfile);
        fputc('0', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
    }
    else if(strcmp(comp, "D-1") == 0 ) {

        fputc('0', outfile);
        fputc('0', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
        fputc('0', outfile);
    }
    else if(strcmp(comp, "A-1") == 0 || strcmp(comp, "M-1") == 0 ) {

        fputc('1', outfile);
        fputc('1', outfile);
        fputc('0', outfile);
        fputc('0', outfile);
        fputc('1', outfile);
        fputc('0', outfile);
    }
    else if(strcmp(comp, "D+A") == 0 || strcmp(comp, "D+M") == 0 ) {
        fputc('0', outfile);
        fputc('0', outfile);
        fputc('0', outfile);
        fputc('0', outfile);
        fputc('1', outfile);
        fputc('0', outfile);
    }
    else if(strcmp(comp, "D-A") == 0 || strcmp(comp, "D-M") == 0 ) {

        fputc('0', outfile);
        fputc('1', outfile);
        fputc('0', outfile);
        fputc('0', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
    }
    else if(strcmp(comp, "A-D") == 0 || strcmp(comp, "M-D") == 0 ) {

        fputc('0', outfile);
        fputc('0', outfile);
        fputc('0', outfile);
        fputc('1', outfile);
        fputc('1', outfile);
        fputc('1', outfile);

    }
    else if(strcmp(comp, "D&A") == 0 || strcmp(comp, "D&M") == 0 ) {
        fputc('0', outfile);
        fputc('0', outfile);
        fputc('0', outfile);
        fputc('0', outfile);
        fputc('0', outfile);
        fputc('0', outfile);

    }
    else if(strcmp(comp, "D|M") == 0 || strcmp(comp, "D|A") == 0 ) {
        fputc('0', outfile);
        fputc('1', outfile);
        fputc('0', outfile);
        fputc('1', outfile);
        fputc('0', outfile);
        fputc('1', outfile);

    }
}











