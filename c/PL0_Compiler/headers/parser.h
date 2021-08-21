#pragma once

#include "symtab.h"
#include "scanner.h"

void program(Token **tokenList);
void block(int lev, int tx);
void constDecl(int lev, int *ptx, int *pdx);
void varDecl(int lev, int *ptx, int *pdx);
void procDecl(int lev, int *ptx, int *pdx);
void statement(int lev, int *ptx);
void condition(int lev, int *ptx);
void relOp(TokenType type);
int isRelop(void);
void expression(int lev, int *ptx);
void term(int lev, int *ptx);
void factor(int lev, int *ptx);
