#pragma once

#include <string.h>
#include "scanner.h"
#include "vm.h"

#define MAX_SYMBOL_TABLE_SIZE 1024
#define CODE_SIZE 1024

typedef struct
{
	int kind;
	char *name;
	int val;
	int level;
	int address;
	int mark;
}Symbol;

Symbol table[MAX_SYMBOL_TABLE_SIZE];
Instruction code[CODE_SIZE];

int tokenIndex,
	//tableIndex,
	rfIndex,
	codeIndex,
	level,
	address;
Token *token;
Token **tokens;

void advanceToken(void);
int ensureType(TokenType type);
void emit(OP opr, int r, int l, int m);
void enter(int kind, char *name, int num, int *ptx, int *pdx, int lev);
void del(char *name);
int lookup(char *name, int *ptx);
void printSymbolTable(int size);
void error(int errorNo);