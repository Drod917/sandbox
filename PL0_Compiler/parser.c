#include "parser.h"

Symbol **parse(Token **tokenList);
void program(void);
void block(void);
void constDecls(void);
void varDecls(void);
void procDecls(void);
void statement(void);
void condition(void);
void expression(int reg);
void term(int reg);
void factor(int reg);

int num1, num2;

// Parses a list of tokens and returns a symbol table if
// syntactically correct.
Symbol **parse(Token **tokenList)
{
	// Initiate
	if (tokenList == NULL)
	{
		printf("TokenList is NULL\n");
		exit(0);
	}
	tokenIndex = -1;
	symbolIndex = 1;
	symbolTable = malloc(sizeof(Symbol) * SYMBOL_TABLE_SIZE);
	if (symbolTable == NULL)
	{
		printf("NULL POINTER ON SYMBOL TABLE CREATION.\n");
		exit(0);
	}

	cx = 0;
	code = malloc(sizeof(Instruction) * CODE_SIZE);
	if (code == NULL)
	{
		printf("NULL POINTER ON SYMBOL TABLE CREATION.\n");
		exit(0);
	}
	int i;
	for (i = 0; i < CODE_SIZE; i++)
	{
		code[i] = malloc(sizeof(Instruction));
	}

	// GET(TOKEN)
	advanceToken();
	block();
	symbolTable[1]->level = -1;
	symbolTable[1]->addr = -1;
	if (!ensureType(periodsym))
	{
		// ERROR
		printf("Error number 9, period expected.\n");
		exit(0);
	}
	emit(SIO3, 0, 0, 3);
	return symbolTable;
}
void block(void)
{
	// constdecl
	if (ensureType(constsym))
	{
		constDecls();

		if (!ensureType(semicolonsym))
		{
			// ERROR
			printf("Error number 5, semicolon or comma missing.\n");
			exit(0);
		}
		advanceToken();
	}

	// vardecl
	if (ensureType(varsym))
	{
		varDecls();

		if (!ensureType(semicolonsym))
		{
			// ERROR
			printf("Error number 5, semicolon or comma missing.\n");
			exit(0);
		}
		advanceToken();
	}
	// procedure
	while (ensureType(procsym))
	{
		//procDecls();
		advanceToken();
		if (!ensureType(identsym))
		{
			printf("Error number 6, incorrect symbol after procedure declaration.\n");
			exit(0);
		}
		advanceToken();
		if (!ensureType(semicolonsym))
		{
			// ERROR
			printf("Error number 5, semicolon or comma missing.\n");
			exit(0);
		}
		advanceToken();
		block();
		if (!ensureType(semicolonsym))
		{
			printf("Error number 5, semicolon or comma missing\n");
			exit(0);
		}
		advanceToken();
	}

	statement();
}
void constDecls(void)
{
	advanceToken();

	char *identifier = malloc(sizeof(char) * MAX_IDENT_LENGTH);
	if (identifier == NULL)
	{
		printf("NULL POINTER ON IDENTIFIER CREATION.\n");
		exit(0);
	}

	if (!ensureType(identsym))
	{
		// ERROR
		printf("Error number 4, const must be followed by an identifier.\n");
		exit(0);
	}
	identifier = currToken->identifier;

	advanceToken();
	if (!ensureType(eqlsym))
	{
		// ERROR
		printf("Error number 3, identifier must be followed by =\n");
		exit(0);
	}
	advanceToken();
	if (!ensureType(numbersym))
	{
		// ERROR
		printf("Error number 2, = must be followed by a number.\n");
		exit(0);
	}
	// Insert
	symbolTable[symbolIndex++] = newSymbol(1, identifier, currToken->number);

	advanceToken();
	if (ensureType(commasym))
		constDecls();
}
void varDecls(void)
{
	advanceToken();
	if (!ensureType(identsym))
	{
		// ERROR
		printf("Error number 4, var must be followed by an identifier.\n");
		exit(0);
	}
	char *identifier = newIdentifier(currToken->identifier);
	// Insert
	symbolTable[symbolIndex++] = newSymbol(2, identifier, 0);

	advanceToken();
	if (ensureType(commasym))
		varDecls();
}
void procDecls(void)
{
	advanceToken();
	if (!ensureType(identsym))
	{
		// ERROR
		printf("Error number 11, undeclared identifier.\n");
		exit(0);
	}
	char *identifier = newIdentifier(currToken->identifier);
	// Insert
	symbolTable[symbolIndex++] = newSymbol(3, identifier, 0);
	address = 4;
	advanceToken();
	if (!ensureType(semicolonsym))
	{
		// ERROR
		printf("Error number 5, semicolon or comma missing.\n");
		exit(0);
	}
	advanceToken();
	level++;
	block();
	if (!ensureType(semicolonsym))
	{
		// ERROR
		printf("Error number 5, semicolon or comma missing.\n");
		exit(0);
	}
	advanceToken();
}
void statement(void)
{
	// identifier
	if (ensureType(identsym))
	{
		advanceToken();
		if (!ensureType(becomessym))
		{
			// ERROR
			printf("Error number 13, assignment operator expected.\n");
			exit(0);
		}
		advanceToken();
		expression(0);	// 0 for starting register R

		// This if condition invokes the requirement that each assignment
		// statement must end with a semicolon.
		// if (!ensureType(semicolonsym))
		// {
		// 	// ERROR
		// 	printf("Error number 5, semicolon or comma missing.\n");
		// 	exit(0);
		// }
	}
	// call
	else if (ensureType(callsym))
	{
		advanceToken();
		if (!ensureType(identsym))
		{
			// ERROR
			printf("Error number 14, call must be followed by an identifier.\n");
			exit(0);
		}
		advanceToken();
	}
	// begin
	else if (ensureType(beginsym))
	{
		advanceToken();
		statement();

		while (ensureType(semicolonsym))
		{
			advanceToken();
			statement();
		}

		if (!ensureType(endsym))
		{
			// ERROR
			printf("Error number 7, statement expected.\n");
			exit(0);
		}
		advanceToken();
	}
	// if
	else if (ensureType(ifsym))
	{
		advanceToken();
		condition();

		if (!ensureType(thensym))
		{
			// ERROR
			printf("Error number 16, then expected.\n");
			exit(0);
		}
		advanceToken();
		int ctemp = cx;
		emit(JPC, 0, 0, 0);
		statement();
		code[ctemp]->m = cx;
	}
	// while
	else if (ensureType(whilesym))
	{
		int cx1 = cx;
		advanceToken();
		condition();
		int cx2 = cx;
		emit(JPC, 0, 0, 0);
		if (!ensureType(dosym))
		{
			// ERROR
			printf("Error number 18, do expected.\n");
			exit(0);
		}
		advanceToken();
		statement();
		emit(JMP, 0, 0, cx1);
		code[cx2]->m = cx;
	}
	// read
	else if (ensureType(readsym))
	{
		advanceToken();
		if (!ensureType(identsym))
		{
			// ERROR
			printf("Error number 19, identifier expected following READ.\n");
			exit(0);
		}
		emit(SIO2, 0, 0, 2);
	}
	// write
	else if (ensureType(writesym))
	{
		advanceToken();
		if (!ensureType(identsym))
		{
			// ERROR
			printf("Error number 20, identifier expected following WRITE.\n");
			exit(0);
		}
		emit(SIO1, 0, 0, 1);
	}
}
void condition(void)
{
	// odd
	if (ensureType(oddsym))
	{
		advanceToken();
		expression(0);
	}
	else
	{
		expression(0);

		if (!isRelop(currToken))
		{
			// ERROR
			printf("Error number 20, relational operator expected.\n");
			exit(0);
		}
		advanceToken();
		expression(0);
	}
}
void expression(int reg)
{
	int left = reg, right = reg + 1;
	TokenType addop;
	// plus or minus
	if (ensureType(plussym) || ensureType(minussym))
	{
		addop = currToken->type;
		advanceToken();
		if (addop == minussym);
			emit(NEG, 0, RF[left], 0);	// negate
	}
	term(left);

	while (ensureType(plussym) || ensureType(minussym))
	{
		addop = currToken->type;
		advanceToken();
		term(right);

		if (addop == plussym)
			emit(ADD, 0, RF[left], RF[right]);	// addition
		else
			emit(SUB, 0, RF[left], RF[right]);	// subtraction
	}
}
void term(int reg)
{
	int left = reg, right = reg + 1;
	int mulop;
	factor(left);
	while (ensureType(multsym) || ensureType(slashsym))
	{
		mulop = currToken->type;
		advanceToken();
		factor(right);
		if (mulop == multsym)
			emit(MUL, 0, RF[left], RF[right]);	// multiplication
		else
			emit(DIV, 0, RF[left], RF[right]);	// division
	}
}
void factor(int reg)
{
	if (ensureType(identsym))
		advanceToken();
	else if (ensureType(numbersym))
	{
		RF[reg] = currToken->number;
		advanceToken();
	}
	else if (ensureType(lparentsym))
	{
		advanceToken();
		expression(reg);

		if (!ensureType(rparentsym))
		{
			// ERROR
			printf("Error number 22, right parenthesis missing.\n");
			exit(0);
		}
	}
	else
	{
		printf("Error number 23, the preceding factor cannot begin with this symbol.\n");
		exit(0);
	}
}