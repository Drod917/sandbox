// Daniel Rodriguez
// COP3402 
// Professor Montagne

#define SYMBOL_TABLE_SIZE 1024
#define MAX_IDENT_LENGTH 12

struct Symbol *newSymbol(int kind, char * identifier, int val);
struct Symbol **symbolTable;
void advanceToken(void);
int ensureType(TokenType type);
void printSymbolTable(int size);

typedef struct Symbol
{
	int kind;		// const = 1, var = 2, proc = 3
	char *name;	// name up to 11 chars
	int val;		// number (ASCII value)
	int level;		// L level
	int addr;		// M address
	int mark;		// to indicate that code has been generated already for a block
}Symbol;

Symbol **symbolTable;
int symbolIndex;
int level = 0;
int address = 3;

void advanceToken()
{
	tokenIndex++;
	currToken = tokenList[tokenIndex];
	
	// DEBUG
	
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

// KIND: const = 1, var = 2, proc = 3
Symbol *newSymbol(int kind, char * identifier, int val)
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
	if (kind == 3)
		newSymbol->addr = -1;
	else
		newSymbol->addr = ++address;

	newSymbol->kind = kind;
	newSymbol->name = identifier;
	newSymbol->val = val;
	newSymbol->level = level;
	newSymbol->mark = 0;

	// printf("ADDING NEW SYMBOL %s\n", identifier);

	return newSymbol;
}

// void enter(int kind, char *name, int val, Token *currToken)
// {
// 	if (currToken == NULL)
// 	{
// 		printf("NULL POINTER ON SYMBOL TABLE INSERTION.\n");
// 		exit(0);
// 	}
// 	if (ensureType(constsym))
// 	{
// 		// enter const symbol to table
// 	}
// 	else if (ensureType(varsym))
// 	{
// 		// enter var symbol to table
// 	}
// 	else if (ensureType(procsym))
// 	{
// 		// enter proc symbol to table
// 	}
// }

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

void printSymbolTable(int size)
{
	printf("\nSymbol Table:\n");
	int i;
	for (i = 1; i < size; i++)
	{
		printf("Kind: %d Name: %s Value: %d Level: %d Address: %d Mark: %d\n",
				symbolTable[i]->kind, symbolTable[i]->name, symbolTable[i]->val,
				symbolTable[i]->level, symbolTable[i]->addr, symbolTable[i]->mark);
	}
	printf("\n");
}

char *newIdentifier(char *name)
{
	if (name == NULL)
	{
		printf("NULL POINTER PASSED TO newIdentifier.\n");
		exit(0);
	}

	char *newIdent = malloc(sizeof(char) * MAX_IDENT_LENGTH);
	if (newIdent == NULL)
	{
		printf("NULL POINTER ON IDENTIFIER ALLOCATION.\n");
		exit(0);
	}

	strcpy(newIdent, name);
	return newIdent;
}