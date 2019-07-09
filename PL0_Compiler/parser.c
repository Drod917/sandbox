#include "parser.h"

Symbol **parse(Token **tokenList);
void block(void);
void statement(void);
void condition(void);
void expression(void);
void term(void);
void factor(void);

// Parses a list of tokens and returns a symbol table if
// syntactically correct.
Symbol **parse(Token **tokenList)
{
	if (tokenList == NULL)
	{
		printf("TokenList is NULL\n");
		exit(0);
	}
	tokenIndex = 0;
	// GET(TOKEN)
	currToken = tokenList[tokenIndex];

	Program *newProgram = malloc(sizeof(Program));
	if (newProgram == NULL)
	{
		printf("NULL POINTER ON PROGRAM CREATION\n");
		exit(0);
	}
	block();

	if (!ensureType(currToken, periodsym))
	{
		// ERROR
		printf("Period expected.\n");
		exit(0);
	}
}

void block(void)
{
	// const
	if (ensureType(currToken, constsym))
	{
		do
		{
			advanceToken();
			if (!ensureType(currToken, identsym))
			{
				// ERROR
				printf("Error number 4, const must be followed by an identifier.\n");
				exit(0);
			}
			advanceToken();
			if (!ensureType(currToken, eqlsym))
			{
				// ERROR
				printf("Error number 3, identifier must be followed by =\n");
				exit(0);
			}
			advanceToken();
			if (!ensureType(currToken, numbersym))
			{
				// ERROR
				printf("Error number 2, = must be followed by a number.\n");
				exit(0);
			}
			advanceToken();
		}
		while (ensureType(currToken, commasym));

		if (!ensureType(currToken, semicolonsym))
		{
			// ERROR
			printf("Error number 5, semicolon or comma missing.\n");
			exit(0);
		}
		advanceToken();
	}

	// var
	else if (ensureType(currToken, varsym))
	{
		do
		{
			advanceToken();
			if (!ensureType(currToken, identsym))
			{
				// ERROR
				printf("Error number 4, var must be followed by an identifier.\n");
				exit(0);
			}
			advanceToken();
		}
		while (ensureType(currToken, commasym));

		if (!ensureType(currToken, semicolonsym))
		{
			// ERROR
			printf("Error number 5, semicolon or comma missing.\n");
			exit(0);
		}
		advanceToken();
	}
	// procedure
	else if(ensureType(currToken, procsym))
	{
		advanceToken();
		if (!ensureType(currToken, identsym))
		{
			// ERROR
			printf("Error number 11, undeclared identifier.\n");
			exit(0);
		}
		advanceToken();
		if (!ensureType(currToken, semicolonsym))
		{
			// ERROR
			printf("Error number 5, semicolon or comma missing.\n");
			exit(0);
		}
		advanceToken();
		block();

		if (!ensureType(currToken, semicolonsym))
		{
			// ERROR
			printf("Error number 5, semicolon or comma missing.\n");
			exit(0);
		}
		advanceToken();
	}
	statement();
}

void statement(void)
{
	// identifier
	if (ensureType(currToken, identsym))
	{
		advanceToken();
		if (!ensureType(currToken, becomessym))
		{
			// ERROR
			printf("Assignment operator expected.\n");
			exit(0);
		}
		advanceToken();
		expression();

		if (!ensureType(currToken, semicolonsym))
		{
			// ERROR
			printf("Error number 5, semicolon or comma missing.\n");
			exit(0);
		}
	}
	// call
	else if (ensureType(currToken, callsym))
	{
		advanceToken();
		if (!ensureType(currToken, identsym))
		{
			// ERROR
			printf("Error number 14, call must be followed by an identifier.\n");
			exit(0);
		}
		advanceToken();
	}
	// begin
	else if (ensureType(currToken, beginsym))
	{
		advanceToken();
		statement();

		while (ensureType(currToken, semicolonsym))
		{
			advanceToken();
			statement();
		}

		if (!ensureType(currToken, endsym))
		{
			// ERROR
			printf("Error number 7, statement expected.\n");
			exit(0);
		}
		advanceToken();
	}
	// if
	else if (ensureType(currToken, ifsym))
	{
		advanceToken();
		condition();

		if (!ensureType(currToken, thensym))
		{
			// ERROR
			printf("Error number 16, then expected.\n");
			exit(0);
		}
		advanceToken();
		statement();
	}
	// while
	else if (ensureType(currToken, whilesym))
	{
		advanceToken();
		condition();

		if (!ensureType(currToken, dosym))
		{
			// ERROR
			printf("Error number 18, do expected.\n");
			exit(0);
		}
		advanceToken();
		statement();
	}
}
void condition(void)
{
	// odd
	if (ensureType(currToken, oddsym))
	{
		advanceToken();
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
	// plus or minus
	if (ensureType(currToken, plussym) || ensureType(currToken, minussym))
		advanceToken();
	term();
	while (ensureType(currToken, plussym) || ensureType(currToken, minussym))
	{
		advanceToken();
		term();
	}
}
void term(void)
{
	factor();
	while (ensureType(currToken, multsym) || ensureType(currToken, slashsym))
	{
		advanceToken();
		factor();
	}
}
void factor(void)
{
	if (ensureType(currToken, identsym))
		advanceToken();
	else if (ensureType(currToken, numbersym))
		advanceToken();
	else if (ensureType(currToken, lparentsym))
	{
		advanceToken();
		expression();

		if (!ensureType(currToken, rparentsym))
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