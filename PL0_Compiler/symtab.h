#define MAX_SYMBOL_TABLE_SIZE 1024
#define CODE_SIZE 1024

void emit(OP opr, int r, int l, int m);
void enter(int kind, char *name, int val, int level, int address);
int lookup(char *name);
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
	tableIndex,
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
		printf("TOKEN IS NOW NULL.\n");
	}
	// else
	// 	printf("Token is now %d\n", token->type);
}
int ensureType(TokenType type)
{
	if (token == NULL)
	{
		printf("Token is NULL!\n");
		exit(0);
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
// Kind: 1 = const, 2 = var
void enter(int kind, char *name, int val, int level, int addr)
{
	address++;
	Symbol newEntry;
	newEntry.kind = kind;
	newEntry.name = malloc(sizeof(char) * MAX_IDENT_LENGTH);
	strcpy(newEntry.name, name);
	newEntry.val = val;
	newEntry.level = level;
	newEntry.address = addr;
	newEntry.mark = 0;

	table[tableIndex++] = newEntry;
}

void del(char *name)
{

}
// aka lookup
int lookup(char *name)
{
	int i;
	for (i = 1; i < tableIndex; i++)
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
}
void error(int errorNo)
{
	switch (errorNo)
	{
		case 1:
			printf("Use = instead of :=.\n");
			break;
		case 2:
			printf("= must be followed by a number.\n");
			break;
		case 3:
			printf("Identifier must be followed by =.\n");
			break;
		case 4:
			printf("const, var, procedure must be followed by identifier.\n");
			break;
		case 5:
			printf("Semicolon or comma missing.\n");
			break;
		case 6:
			printf("Incorrect symbol after procedure declaration.\n");
			break;
		case 7:
			printf("Statement expected.\n");
			break;
		case 8:
			printf("Incorrect symbol after statement part in block.\n");
			break;
		case 9:
			printf("Period expected.\n");
			break;
		case 10:
			printf("Semicolon between statements missing.\n");
			break;
		case 11:
			printf("Undeclared identifier.\n");
			break;
		case 12:
			printf("Assignment to constant or procedure is not allowed.\n");
			break;
		case 13:
			printf("Assignment operator expected.\n");
			break;
		case 14:
			printf("call must be followed by an identifier.\n");
			break;
		case 15:
			printf("Call of a constant or variable is meaningless.\n");
			break;
		case 16:
			printf("then expected.\n");
			break;
		case 17:
			printf("Semicolon or } expected.\n");
			break;
		case 18:
			printf("do expected.\n");
			break;
		case 19:
			printf("Incorrect symbol following statement.\n");
			break;
		case 20:
			printf("Relational operator expected.\n");
			break;
		case 21:
			printf("Expression must not contain a procedure identifier.\n");
			break;
		case 22:
			printf("Right parenthesis is missing.\n");
			break;
		case 23:
			printf("The preceding factor cannot begin with this symbol.\n");
			break;
		case 24:
			printf("An expression cannot begin with this symbol.\n");
			break;
		case 25:
			printf("Code size exceeds maximum.\n");
			break;
		case 26:
			printf("end expected to close begin statement.\n");
			break;
		case 27:
			printf("Factor must be a number or identifier.\n");
			break;
		default:
			break;
	}
	exit(0);
}