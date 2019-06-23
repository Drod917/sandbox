#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// 11 is the max, the 1 is for the null sentinel.
#define MAX_IDENT_LEN (11 + 1)
#define BUFFER_LEN 32

enum TokenType;
struct Token;
struct TokenListElement;
struct TokenList;
struct TokenListElement *newTokenListElement(struct Token);
struct Token *newToken(enum TokenType type, char * ident);
void init();

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
	Token *element;
	struct TokenListElement *next;
}TokenListElement;

struct TokenList
{
	TokenListElement *head, *tail;
}tokens;

FILE *ifp;
char *buffer;
int debug = 1;


// Creates a new token, then links it with the running list of tokens
void addToken(TokenType type, char * ident)
{

	if (debug)
	{
		fprintf(stdout, "Adding Token %s...\n", ident);
	}

	Token *newToken = malloc(sizeof(Token));
	if (newToken == NULL)
	{
		fprintf(stderr, "NULL POINTER ON NEW TOKEN");
		return;
	}
	newToken->type = type;
	newToken->identifier = ident;

	TokenListElement *newElement = malloc(sizeof(TokenListElement));
	newElement->element = newToken;
	newElement->next = NULL;

	// Link with running list

	// Does the head exist?
	if (tokens.head == NULL)
	{
		tokens.head = newElement;
		tokens.tail = newElement;
		return;
	}

	tokens.tail->next = newElement;
	tokens.tail = newElement;

	return;
}
void init(int argc, char *filename)
{
	// Improper syntax detected
	if (argc != 2)
	{
		fprintf(stderr, "CORRECT SYNTAX: 'scan <input_file.txt>'\n");
		exit(0);
	}
	ifp = fopen(filename, "r");
	// File not found
	if (ifp == NULL)
	{
		fprintf(stderr, "FILE '%s' NOT FOUND\n", filename);
		exit(0);
	}

	buffer = malloc(sizeof(char) * BUFFER_LEN);
	if (buffer == NULL)
		exit(0);

	// Load the buffer
	fscanf(ifp, "%s", buffer);
}

// Tokenize what's in the buffer
void tokenize(void)
{
	// constdeclaration 
	// ::= [ “const” ident "=" number {"," ident "=" number} “;"].
	if (strcmp(buffer, "const") == 0)
	{
		addToken(constsym, buffer);

		fscanf(ifp, "%s", buffer);
		addToken(identsym, buffer);

		fscanf(ifp, "%s", buffer);
		addToken(eqlsym, "=");

		// TODO: CHECK FOR NUM?
		int x;
		fscanf(ifp, "%d", &x);
		char y = x + '0';
		char *c = malloc(sizeof(char) + 1);
		*c = y;
		addToken(numbersym, c);

		fscanf(ifp, "%s", buffer);
		fprintf(stdout, "Buffer is %s\n", buffer);
		while (strcmp(buffer, ";") != 0);
		{
				addToken(commasym, ",");

				fscanf(ifp, "%s", buffer);
				addToken(identsym, buffer);
				// link identTok2

				addToken(eqlsym, "=");

				fscanf(ifp, "%s", buffer);
				addToken(numbersym, buffer);

				fscanf(ifp, "%s", buffer);
		}
		addToken(semicolonsym, ";");
	}
}

void printTokens()
{
	TokenListElement *traverse = tokens.head;

	while (traverse != NULL)
	{
		fprintf(stdout, "%s\t%d", traverse->element->identifier,
				traverse->element->type);
		fprintf(stdout, "\n");
		traverse = traverse->next;
	}
}

int main(int argc, char **argv)
{
	init(argc, argv[1]);

	// Begin Scanning
	while (!feof(ifp))
	{
		tokenize();
		fscanf(ifp, "%s", buffer);
	}

	printTokens();
	printf("\n");
	return 0;
}