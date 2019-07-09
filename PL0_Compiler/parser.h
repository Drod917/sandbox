// Daniel Rodriguez
// COP3402 
// Professor Montagne

#define MAX_SYMBOL_TABLE_SIZE 1024

struct Symbol **symbolTable;
struct Program;
struct Block;
struct FuncDecl;
struct TypedIdent;
struct TypedIdentList;
struct FuncDeclList;
struct Statement;
struct StatementList;
struct Expression;
struct ExpressionList;

typedef struct Symbol
{
	int kind;		// const = 1, var = 2, proc = 3
	char name[10];	// name up to 11 chars
	int val;		// number (ASCII value)
	int level;		// L level
	int addr;		// M address
	int mark;		// to indicate that code has been generated already for a block
}Symbol;

typedef struct Program
{
	struct Block *block;
}Program;

typedef struct Block
{
	struct ConstDecl *constDecls;
	struct VarDecl *varDecls;
	struct Statement **statementList;
}Block;

typedef struct Statement
{

}Statement;

void advanceToken();
Symbol *newSymbol(Token *currToken);
Token **tokenList;
Token *currToken;
int tokenIndex;
Symbol **symbolTable;

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

int ensureType(Token *token, TokenType type)
{
	if (token == NULL)
	{
		return 0;
	}
	if (token->type != type)
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