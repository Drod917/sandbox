#include "parser.h"

Symbol **parse(Token **tokenList);
void program(void);
void block(void);
void constDecls(void);
void varDecls(void);
void procDecls(void);
void statement(void);
void condition(void);
void expression(void);
void term(void);
void factor(void);

int level = 0;
int address = 0;

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

	if (!ensureType(periodsym))
	{
		// ERROR
		printf("Period expected.\n");
		exit(0);
	}

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
	if(ensureType(procsym))
	{
		procDecls();
		
		if (!ensureType(semicolonsym))
		{
			// ERROR
			printf("Error number 5, semicolon or comma missing.\n");
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

	advanceToken();
	if (!ensureType(semicolonsym))
	{
		// ERROR
		printf("Error number 5, semicolon or comma missing.\n");
		exit(0);
	}
	advanceToken();
	block();
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
			printf("Assignment operator expected.\n");
			exit(0);
		}
		advanceToken();
		expression();

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
}
void condition(void)
{
	// odd
	if (ensureType(oddsym))
	{
		advanceToken();
		expression();
	}
	else
	{
		expression();

		if (!isRelop(currToken))
		{
			// ERROR
			printf("Error number 20, relational operator expected.\n");
			exit(0);
		}
		advanceToken();
		expression();
	}
}
void expression(void)
{
	TokenType addop;
	// plus or minus
	if (ensureType(plussym) || ensureType(minussym))
	{
		addop = currToken->type;
		advanceToken();
		//if (addop == minussym);
			//emit(OPR, 0, OPR_NEG);	// negate
	}
	term();

	while (ensureType(plussym) || ensureType(minussym))
	{
		advanceToken();
		term();
	}
}
void term(void)
{
	factor();
	while (ensureType(multsym) || ensureType(slashsym))
	{
		advanceToken();
		factor();
	}
}
void factor(void)
{
	if (ensureType(identsym))
		advanceToken();
	else if (ensureType(numbersym))
		advanceToken();
	else if (ensureType(lparentsym))
	{
		advanceToken();
		expression();

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

