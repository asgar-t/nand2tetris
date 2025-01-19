
#ifndef HACK_ASSEMBLER_H
#define HACK_ASSEMBLER_H

#define MAX_SYMBOLS 1003
#define SYM_LEN 80
#define MAX_LINE 300


void parse_symbol(char line[]);
void parse_variable(char line[]);
void parse_instruction(char line[], FILE* outfile);

int sym_add(char symbol[], int value);
int sym_check(char symbol[]);
int sym_get(char symbol[]);
void table_initialize();


void a_instr(char instruction[], FILE* outfile);
void dest_instr(char dest[], FILE* outfile);
void comp_instr(char comp[], FILE* outfile);
void jmp_instr(char jump[], FILE* outfile);

int contains_char(char line[], char character);

#endif //HACK_ASSEMBLER_H
