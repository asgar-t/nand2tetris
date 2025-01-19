#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hack_assembler.h"
//bucket data type
typedef struct bucket {
    char symbol[SYM_LEN];
    int value;
    struct bucket *next;

}bucket;
//hash function
unsigned hash(char s[]) { //hash function given from project details
    unsigned hashval, i;
    for (hashval = 0, i = 0; s[i] != '\0'; i++) {
        hashval += s[i] + 31*hashval;
    }
    return hashval % MAX_SYMBOLS;
}
//hashtable
bucket sym_table[MAX_SYMBOLS];

//initializes up hashtable
void table_initialize(){

    char sym[SYM_LEN];

    //clears hashtable
    int i;
    for (i = 0; i < MAX_SYMBOLS; i++) {
        sym_table[i].symbol[0] = '\0';
        sym_table[i].value = 0;
        sym_table[i].next = NULL;
    }
    //sets first 16 known symbols R0...R15
    for (i = 0; i < 10; i++) {
        sym[0] = 'R';
        sym[1] = '0' + i;
        sym[2] = '\0';
        sym_add(sym, i);

    }
    for (i = 0; i < 6; i++) {
        sym[0] = 'R';
        sym[1] = '1';
        sym[2] = '0' + i;
        sym[3] = '\0';
        sym_add(sym, 10+i);
    }
    //sets other known symbols
    sym_add("SCREEN", 16384);
    sym_add("KBD", 24576);
    sym_add("SP", 0);
    sym_add("LCL", 1);
    sym_add("ARG", 2);
    sym_add("THIS", 3);
    sym_add("THAT", 4);
}
//adds symbol to symbol table
int sym_add(char *symbol, int value) {

    int in_table = sym_check(symbol);


    if(!in_table) {
        //if the symbol is not already in the table
        bucket *current_sym = &sym_table[hash(symbol)];
        if (current_sym->symbol[0] == '\0') {
            strcpy(current_sym -> symbol, symbol);
            current_sym->value = value;
            return 1;
        }
        else{

            while(current_sym -> next != NULL) {
                current_sym = current_sym -> next;

            }
            //new bucket
            bucket *new_sym = malloc(sizeof(bucket));
            current_sym -> next = new_sym;
            strcpy(new_sym->symbol, symbol);
            new_sym->value = value;
            new_sym->next = NULL;

            return 1;
        }
    }
    return 0;
}

//checks if symbol is in the table
int sym_check(char *symbol) {
    bucket *current_sym = &sym_table[hash(symbol)];
    if (current_sym -> symbol[0] == '\0') {
        return 0;

    }
    if (strcmp(current_sym->symbol, symbol) == 0) {
        return 1;
    }

    current_sym = current_sym->next;
    while (current_sym != NULL) {
        if (strcmp(symbol, current_sym->symbol) == 0) {
            return 1;
        }
        current_sym = current_sym->next;

    }
    return 0;

}

//returns value associated with a given symbol
int sym_get(char symbol[]) {
    bucket *current_sym = &sym_table[hash(symbol)];
    while(current_sym != NULL) {
        if (strcmp(symbol, current_sym->symbol) == 0) {
            return current_sym->value;
        }
        current_sym = current_sym->next;
    }
    return 0;
}