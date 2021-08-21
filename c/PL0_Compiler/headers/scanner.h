// Daniel Rodriguez
// Professor Montagne
// COP 3402 Spring 2019 (NID da268008)
#pragma once

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 1024
#define MAX_IDENT_LENGTH 12

typedef enum TokenType
{
	
	nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5, multsym = 6,
	slashsym = 7, oddsym = 8, eqlsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
	gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17,
	semicolonsym = 18, periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22,
	ifsym = 23, thensym = 24, whilesym = 25, dosym = 26, callsym = 27, constsym = 28,
	varsym = 29, procsym = 30, writesym = 31, readsym = 32, elsesym = 33
	
}TokenType;

typedef struct Token
{
	enum TokenType type;
	char *identifier;
	int number;
}Token;

Token **tokenList;
Token *currToken;
Token **scan(FILE *ifp);
Token *allocToken();
Token **allocTokenList();
Token *newIdent(int type, char *identifier);
Token *newNumber(int type, int number);
Token *newToken(int type);
FILE *ifp;
int isReserved(char *identifier);
char *getType(int type);
void printTokenList(Token **tokenList);
void initScanner(char *filename);