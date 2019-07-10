// Daniel Rodriguez
// COP3402 
// Professor Montagne

#define MAX_SYMBOL_TABLE_SIZE 1024

struct Symbol **symbolTable;
struct Program;
struct varDecl;
struct procDecl;
struct statement;
void advanceToken();
int ensureType(TokenType type);

typedef struct Symbol
{
	int kind;		// const = 1, var = 2, proc = 3
	char name[10];	// name up to 11 chars
	int val;		// number (ASCII value)
	int level;		// L level
	int addr;		// M address
	int mark;		// to indicate that code has been generated already for a block
}Symbol;

Symbol *newSymbol(Token *currToken);
Token **tokenList;
Token *currToken;
int tokenIndex;
Symbol **symbolTable;


void constDecls(void)
{
	advanceToken();

	do
	{
		if (!ensureType(identsym))
		{
			// ERROR
		}
		advanceToken();
		if (!ensureType(eqlsym))
		{
			// ERROR
		}
		advanceToken();
		if (!ensureType(numbersym))
		{
			// ERROR 
		}

		// ENTER SYMBOL INTO SYMBOL TABLE
		// enter(constant, ident, number);
		advanceToken();
	}
	while (ensureType(commasym));

	if (!ensureType(semicolonsym))
	{
		// ERROR
	}
	advanceToken();
}

void varDecls(void)
{

}

void procDecls(void)
{

}


void advanceToken()
{
	tokenIndex++;
	currToken = tokenList[tokenIndex];
	
	// DEBUG
	//
	// if (currToken == NULL)
	// {
	// 	printf("\nCURRTOKEN IS NOW NULL\n");
	// 	exit(0);
	// }
	// if (currToken->type == numbersym)
	// {
	// 	printf("\nToken is now %d\n", currToken->number);
	// 	return;
	// }
	// else if (currToken->identifier != NULL)
	// {
	// 	printf("\nToken is now %s\n", currToken->identifier);
	// }
	// else 
	// 	printf("\nToken is now %d\n", currToken->type);

}

Symbol *newSymbol(Token *currToken)
{
	if (currToken == NULL)
	{
		printf("NULL TOKEN ON SYMBOL CREATION\n");
		exit(0);
	}

	Symbol *newSymbol = malloc(sizeof(Symbol));
	if (newSymbol == NULL)
	{
		printf("NULL POINTER ON SYMBOL CREATION\n");
		exit(0);
	}

	if (currToken->type == constsym)
	{
		newSymbol->kind = 1;
	}
}

void enter(Token *currToken)
{
	if (currToken == NULL)
	{
		printf("NULL POINTER ON SYMBOL TABLE INSERTION.\n");
		exit(0);
	}
	if (ensureType(constsym))
	{
		// enter const symbol to table
	}
	else if (ensureType(varsym))
	{
		// enter var symbol to table
	}
	else if (ensureType(procsym))
	{
		// enter proc symbol to table
	}
}

int ensureType(TokenType type)
{
	if (currToken == NULL)
	{
		return 0;
	}
	if (currToken->type != type)
		return 0;

	return 1;
}

int isRelop(Token *token)
{
	if (token == NULL)
		return 0;
	if (token->type == eqlsym ||
		token->type == neqsym ||
		token->type == lessym ||
		token->type == leqsym ||
		token->type == gtrsym ||
		token->type == geqsym)
		return 1;
	return 0;
}