// Daniel Rodriguez
// Professor Montagne
// COP 3402 Spring 2019 (NID da268008)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

struct Token *allocToken();
struct Token **allocTokenList();
struct Token *newIdent(int type, char *identifier);
struct Token *newNumber(int type, int number);
struct Token *newToken(int type);
struct Token **readTokens(FILE *ifp);
int isReserved(char *identifier);
char *getType(int type);
void printTokenList(Token **tokenList);

FILE *ifp;
int tokenIndex;
int debug = 0;

Token *allocToken()
{
	Token *token = malloc(sizeof(Token));
	if (token == NULL)
	{
		fprintf(stderr, "MALLOC FAIL ON NEW TOKEN\n");
		exit(0);
	}
	return token;
}

Token **allocTokenList()
{
	Token **tokenList = malloc(sizeof(Token *) * MAX_TOKENS);
	if (tokenList == NULL)
	{		
		fprintf(stderr, "MALLOC FAIL ON NEW TOKENLIST\n");
		exit(0);
	}
}

Token *newToken(int type)
{
	Token *token = allocToken();
	token->type = type;
	token->identifier = getType(type);
	token->number = 0;

	if (debug)
	{
		fprintf(stdout, "Adding reserved token %d...\n", token->type);
	}
	return token;
}

Token *newNumber(int type, int number)
{
	if (number > 99999)
	{
		fprintf(stderr, "Number %d too long!\n", number);
		exit(0);
	}
	
	Token *token = allocToken();
	token->type = type;

	char *numAsIdent = malloc(sizeof(char) * MAX_IDENT_LENGTH);
	token->identifier = NULL;
	token->number = number;

	if (debug)
	{
		fprintf(stdout, "Adding Token %d...\n", token->number);
	}
	return token;
}

Token *newIdent(int type, char *identifier)
{
	if (identifier == NULL)
	{
		fprintf(stderr, "NULL IDENTIFIER ON NEW IDENT\n");
		exit(0);
	}
	Token *token = allocToken();
	token->type = type;
	token->number = 0;
	int identLength = strlen(identifier) + 1;
	if (identLength > MAX_IDENT_LENGTH)
	{
		fprintf(stderr, "identifier '%s' too long\n", identifier);
		exit(0);
	}
	char *scannedIdent = malloc(sizeof(char) * identLength);
	if (scannedIdent == NULL)
	{
		fprintf(stderr, "MALLOC FAIL ON NEW scannedIdent\n");
		exit(0);
	}
	strncpy(scannedIdent, identifier, identLength);
	token->identifier = scannedIdent;

	if (debug)
	{
		fprintf(stdout, "Adding Token %s...\n", token->identifier);
	}

	return token;
}

int isReserved(char *identifier)
{
	if (identifier == NULL)
	{
		fprintf(stderr, "IDENTIFIER %s IS NULL\n", identifier);
		exit(0);
	}

	if (debug)
	{
		printf("Checking %s...\n\n", identifier);
	}

	if (strcmp(identifier, "odd") == 0)
		return 8;
	else if (strcmp(identifier, "const") == 0)
		return 28;
	else if (strcmp(identifier, "var") == 0)
		return 29;
	else if (strcmp(identifier, "procedure") == 0)
		return 30;
	else if (strcmp(identifier, "begin") == 0)
		return 21;
	else if (strcmp(identifier, "end") == 0)
		return 22;
	else if (strcmp(identifier, "if") == 0)
		return 23;
	else if (strcmp(identifier, "then") == 0)
		return 24;
	else if (strcmp(identifier, "else") == 0)
		return 33;
	else if (strcmp(identifier, "read") == 0)
		return 32;
	else if (strcmp(identifier, "write") == 0)
		return 31;
	else if (strcmp(identifier, "call") == 0)
		return 27;
	else if (strcmp(identifier, "while") == 0)
		return 25;
	else if (strcmp(identifier, "do") == 0)
		return 26;

	return -1;
}

/*
nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5, multsym = 6,
slashsym = 7, oddsym = 8, eqlsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17,
semicolonsym = 18, periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22,
ifsym = 23, thensym = 24, whilesym = 25, dosym = 26, callsym = 27, constsym = 28,
varsym = 29, procsym = 30, writesym = 31, readsym = 32, elsesym = 33	
*/
char *getType(int reservedType)
{
	char *ident = malloc(sizeof(char) * MAX_IDENT_LENGTH);

	switch (reservedType)
	{
		case 4:
			strcpy(ident, "+");
			break;
		case 5:
			strcpy(ident, "-");
			break;
		case 6:
			strcpy(ident, "*");
			break;
		case 7:
			strcpy(ident, "/");
			break;
		case 8:
			strcpy(ident, "odd");
			break;
		case 9:
			strcpy(ident, "=");
			break;
		case 10:
			strcpy(ident, "<>");
			break;
		case 11:
			strcpy(ident, "<");
			break;
		case 12:
			strcpy(ident, "<=");
			break;
		case 13:
			strcpy(ident, ">");
			break;
		case 14:
			strcpy(ident, ">=");
			break;
		case 15:
			strcpy(ident, "(");
			break;
		case 16:
			strcpy(ident, ")");
			break;
		case 17:
			strcpy(ident, ",");
			break;
		case 18:
			strcpy(ident, ";");
			break;
		case 19:
			strcpy(ident, ".");
			break;
		case 20:
			strcpy(ident, ":=");
			break;
		case 21:
			strcpy(ident, "begin");
			break;
		case 22:
			strcpy(ident, "end");
			break;
		case 23:
			strcpy(ident, "if");
			break;
		case 24:
			strcpy(ident, "then");
			break;
		case 25:
			strcpy(ident, "while");
			break;
		case 26:
			strcpy(ident, "do");
			break;
		case 27:
			strcpy(ident, "call");
			break;
		case 28:
			strcpy(ident, "const");
			break;
		case 29:
			strcpy(ident, "var");
			break;
		case 30:
			strcpy(ident, "procedure");
			break;
		case 31:
			strcpy(ident, "write");
			break;
		case 32:
			strcpy(ident, "read");
			break;
		case 33:
			strcpy(ident, "else");
			break;
	}

	return ident;
}


void printTokenList(Token **tokenList)
{
	if (tokenList == NULL)
	{
		fprintf(stderr, "NULL POINTER FOUND TRYING TO PRINT TOKEN LIST\n");
		exit(0);
	}

	fprintf(stdout, "\nLexeme Table:\n\nLexeme\t    Token Type\n");

	// Print Lexeme Table
	int i;
	for (i = 0; i < tokenIndex; i++)
	{
		if (tokenList[i]->identifier == NULL)
		{
			fprintf(stdout, "%-12d%d\n",
					tokenList[i]->number, tokenList[i]->type);

		}
		else
			fprintf(stdout, "%-12s%d\n",
					tokenList[i]->identifier, tokenList[i]->type);
	}

	// Print Lexeme List
	fprintf(stdout, "\nLexeme List:\n");
	for (i = 0; i < tokenIndex; i++)
	{
		// Ident token
		if (tokenList[i]->type == identsym)
		{
			fprintf(stdout, "%d %s%s",
					tokenList[i]->type, tokenList[i]->identifier,
					(i < tokenIndex) ? " " : "\n");
		
		}
		// Number token
		else if (tokenList[i]->type == numbersym)
		{
			fprintf(stdout, "%d %d%s",
					tokenList[i]->type, tokenList[i]->number,
					(i < tokenIndex) ? " " : "\n");
		}
		// Reserved token
		else
			fprintf(stdout, "%d%s",
					tokenList[i]->type,
					(i < tokenIndex) ? " " : "\n");
	}
	fprintf(stdout, "\n");
}