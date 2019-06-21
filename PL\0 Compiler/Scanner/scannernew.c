#include <stdlib.h>
#include <stdio.h>

// 11 is the max, the 1 is for the null sentinel.
#define MAX_IDENT_LEN (11 + 1)
#define BUFFER_LEN 32

enum TokenType;
struct Token;
struct TokenListElement;
struct TokenList;
struct TokenListElement *newTokenListElement(struct Token newToken);

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
	char *identifier;
	TokenType type;
}Token;

typedef struct TokenListElement
{
	Token *listElement;
	Token *next;
}TokenListElement;

struct TokenList
{
	Token *head, *tail;
}TokenList;

FILE *ifp;

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
	
	char *buffer = malloc(sizeof(char) * BUFFER_LEN);
	if (buffer == NULL)
		return 0;
	// Begin Scanning
	while (!feof(ifp))
	{
		fscanf(ifp, "%s", buffer);
		fprintf(stdout, "%s ",buffer);
	}
	printf("\n");
	return 0;
}