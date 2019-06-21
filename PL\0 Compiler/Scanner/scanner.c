// Daniel Rodriguez (NID da268008)
// COP3402 (Summer)
// Professor Montagne

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN 16

typedef enum
{
	nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5, multsym = 6,
	slashsym = 7, oddsym = 8, eqlsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
	gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17,
	semicolonsym = 18, periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22,
	ifsym = 23, thensym = 24, whilesym = 25, dosym = 26, callsym = 27, constsym = 28,
	varsym = 29, procsym = 30, writesym = 31, readsym = 32, elsesym = 33
}token_type;

typedef struct Token
{
	char *identifier;
	token_type tokType;
	struct Token *next;
}Token;

typedef struct TokenList
{
	Token *head, *tail;
}TokenList;


FILE *ifp;
TokenList *tokens;

Token * tokenize(char *currStr)
{
	if (currStr == NULL)
	{
		fprintf(stderr, "NULL TOKEN STR %s, EXITING", currStr);
		exit(0);
	}

	Token *newToken = malloc(sizeof(Token));
	if (newToken == NULL)
	{
		fprintf(stderr, "NEWTOKEN CREATION FAILED");
		exit(0);
	}

	newToken->identifier = currStr;
	// const
	if (strcmp(currStr, "const") == 0)
	{
		newToken->tokType = constsym;
	}
	// var
	else if (strcmp(currStr, "var") == 0)
	{

	}
	// procedure
	else if (strcmp(currStr, "procedure") == 0)
	{

	}
	// call
	else if (strcmp(currStr, "call") == 0)
	{

	}
	// begin
	else if (strcmp(currStr, "begin") == 0)
	{

	}
	// end
	else if (strcmp(currStr, "end") == 0)
	{

	}
	// if
	else if (strcmp(currStr, "if") == 0)
	{

	}
	// then
	else if (strcmp(currStr, "then") == 0)
	{

	}
	// else
	else if (strcmp(currStr, "else") == 0)
	{

	}
	// while
	else if (strcmp(currStr, "while") == 0)
	{

	}
	// do
	else if (strcmp(currStr, "do") == 0)
	{

	}
	// read
	else if (strcmp(currStr, "read") == 0)
	{

	}
	// write
	else if (strcmp(currStr, "write") == 0)
	{

	}
}
int main(int argc, char **argv)
{
	// Improper syntax detected
	if (argc != 2)
	{
		fprintf(stderr, "CORRECT SYNTAX: 'scan <input_file.txt>'\n");
		return 0;
	}
	ifp = fopen(argv[1], "r");
	// File not found
	if (ifp == NULL)
	{
		fprintf(stderr, "FILE '%s' NOT FOUND\n", argv[1]);
		return 0;
	}
	tokens = malloc(sizeof(TokenList));
	if (tokens == NULL)
	{
		fprintf(stderr, "TOKENLIST CREATION FAILED\n");
		return 0;
	}






	// C is first word in file
	char *currStr = malloc(sizeof(char) * BUFFER_LEN);
	fscanf(ifp, "%s", currStr);
	Token *headToken = tokenize(currStr);
	headToken->next = NULL;
	tokens->head = headToken;
	tokens->tail = headToken;
	// while (!feof(ifp))
	// {
	// 	printf("%s",currStr);
	// 	fscanf(ifp, "%s", currStr);
	// }
	if (feof(ifp))
		fprintf(stdout, "End of file reached\n");
	fprintf(stdout, "%s\t%d\n", tokens->head->identifier,
			tokens->head->tokType);
	return 0;
}

/*
Write source
Preprocess source
Compile source -> assembly code
Assembler assembly -> object code
Linker linkers obj files -> executable
Loader executes executable, creates environment to run
*/