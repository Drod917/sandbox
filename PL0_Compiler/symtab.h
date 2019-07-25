#include <string.h>

#define MAX_SYMBOL_TABLE_SIZE 1024
#define MAX_LEXI_LEVELS 3
#define CODE_SIZE 1024

void emit(OP opr, int r, int l, int m);
void enter(int kind, char *name, int num, int *ptx, int *pdx, int lev);
//void enter(int kind, char *name, int val, int level, int address);
int lookup(char *name, int *ptx);
void del(char *name);
void advanceToken(void);
void printSymbolTable(int size);
int ensureType(TokenType type);
void error(int errorNo);

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

void advanceToken(void)
{
	tokenIndex++;
	token = tokenList[tokenIndex];
	if (token == NULL)
	{
		// NULL PTR ERROR
		//printf("TOKEN IS NOW NULL.\n");
	}
	// else
	// 	printf("Token is now %d\n", token->type);
}
int ensureType(TokenType type)
{
	if (token == NULL)
	{
		return 0;
	}
	if (token->type != type)
		return 0;
	return 1;
}
void emit(OP opr, int r, int l, int m)
{
	if (codeIndex > CODE_SIZE)
		error(25);
	else
	{
		code[codeIndex].op = opr;	// opcode
		code[codeIndex].r = r;		// register
		code[codeIndex].l = l;		// lexicographical level
		code[codeIndex].m = m;		// modifier
		codeIndex++;
	}
}

void enter(int kind, char *name, int num, int *ptx, int *pdx, int lev)
{
	(*ptx)++;
	table[*ptx].kind = kind;
	table[*ptx].name = malloc(sizeof(char) * MAX_IDENT_LENGTH);
	strcpy(table[*ptx].name, name);
	if (kind == 1)		// const
		table[*ptx].val = num;
	else if (kind == 2)	// var
	{
		table[*ptx].level = lev;
		table[*ptx].address = (*pdx);
		(*pdx)++;
	}
	else				// proc
		table[*ptx].level = lev;
}
// // Kind: 1 = const, 2 = var, 3 = procedure
// void enter(int kind, char *name, int val, int level, int addr)
// {
// 	address++;
// 	Symbol newEntry;
// 	newEntry.kind = kind;
// 	newEntry.name = malloc(sizeof(char) * MAX_IDENT_LENGTH);
// 	strcpy(newEntry.name, name);
// 	newEntry.val = val;
// 	newEntry.level = level;
// 	newEntry.address = addr;
// 	newEntry.mark = 0;

// 	table[tableIndex++] = newEntry;
// }

void del(char *name)
{

}
// aka lookup
int lookup(char *name, int *ptx)
{
	int i, n = (*ptx);
	for (i = 1; i <= n; i++)
	{
		if (strcmp(table[i].name, name) == 0)
			return i;
	}
	return 0;
}
void printSymbolTable(int size)
{
	int i;
	for (i = 1; i < size; i++)
	{
		printf("Kind: %d Name: %s Val: %d Level: %d Addr: %d Mark: %d\n",
				table[i].kind, table[i].name, table[i].val, table[i].level,
				table[i].address, table[i].mark);
	}
	printf("\n");
}
void error(int errorNo)
{
	switch (errorNo)
	{
		case 1:
			printf("Error number 1. Use = instead of :=.\n");
			break;
		case 2:
			printf("Error number 2. = must be followed by a number.\n");
			break;
		case 3:
			printf("Error number 3. Identifier must be followed by =.\n");
			break;
		case 4:
			printf("Error number 4. const, var, procedure must be followed by an identifier.\n");
			break;
		case 5:
			printf("Error number 5. Semicolon or comma missing.\n");
			break;
		case 6:
			printf("Error number 6. Incorrect symbol after procedure declaration.\n");
			break;
		case 7:
			printf("Error number 7. Statement expected.\n");
			break;
		case 8:
			printf("Error number 8. Incorrect symbol after statement part in block.\n");
			break;
		case 9:
			printf("Error number 9. Period expected.\n");
			break;
		case 10:
			printf("Error number 10. Semicolon between statements missing.\n");
			break;
		case 11:
			printf("Error number 11. Undeclared identifier.\n");
			break;
		case 12:
			printf("Error number 12. Assignment to constant or procedure is not allowed.\n");
			break;
		case 13:
			printf("Error number 13. Assignment operator expected.\n");
			break;
		case 14:
			printf("Error number 14. call must be followed by an identifier.\n");
			break;
		case 15:
			printf("Error number 15. Call of a constant or variable is meaningless.\n");
			break;
		case 16:
			printf("Error number 16. then expected.\n");
			break;
		case 17:
			printf("Error number 17. Semicolon or } expected.\n");
			break;
		case 18:
			printf("Error number 18. do expected.\n");
			break;
		case 19:
			printf("Error number 19. Incorrect symbol following statement.\n");
			break;
		case 20:
			printf("Error number 20. Relational operator expected.\n");
			break;
		case 21:
			printf("Error number 21. Expression must not contain a procedure identifier.\n");
			break;
		case 22:
			printf("Error number 22. Right parenthesis is missing.\n");
			break;
		case 23:
			printf("Error number 23. The preceding factor cannot begin with this symbol.\n");
			break;
		case 24:
			printf("Error number 24. An expression cannot begin with this symbol.\n");
			break;
		case 25:
			printf("Error number 25. Code size exceeds maximum.\n");
			break;
		case 26:
			printf("Error number 26. end expected to close begin statement.\n");
			break;
		case 27:
			printf("Error number 27. Factor must be a number or identifier.\n");
			break;
		default:
			break;
	}
	exit(0);
}