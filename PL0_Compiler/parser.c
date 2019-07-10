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

	block();

	if (!ensureType(periodsym))
	{
		// ERROR
		printf("Period expected.\n");
		exit(0);
	}
}

void block(void)
{
	// constdecl
	if (ensureType(constsym))
	{
		do
		{
			advanceToken();
			if (!ensureType(identsym))
			{
				// ERROR
				printf("Error number 4, const must be followed by an identifier.\n");
				exit(0);
			}
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
			advanceToken();
		}
		while (ensureType(commasym));

		if (!ensureType(semicolonsym))
		{
			// ERROR
			printf("Error number 5, semicolon or comma missing.\n");
			exit(0);
		}
		advanceToken();
	}

	// vardecl
	else if (ensureType(varsym))
	{
		do
		{
			advanceToken();
			if (!ensureType(identsym))
			{
				// ERROR
				printf("Error number 4, var must be followed by an identifier.\n");
				exit(0);
			}
			advanceToken();
		}
		while (ensureType(commasym));

		if (!ensureType(semicolonsym))
		{
			// ERROR
			printf("Error number 5, semicolon or comma missing.\n");
			exit(0);
		}
		advanceToken();
	}
	// procedure
	else if(ensureType(procsym))
	{
		advanceToken();
		if (!ensureType(identsym))
		{
			// ERROR
			printf("Error number 11, undeclared identifier.\n");
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

		if (!ensureType(semicolonsym))
		{
			// ERROR
			printf("Error number 5, semicolon or comma missing.\n");
			exit(0);
		}
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
		statement();
	}
	// while
	else if (ensureType(whilesym))
	{
		advanceToken();
		condition();

		if (!ensureType(dosym))
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
	if (ensureType(oddsym))
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
	if (ensureType(plussym) || ensureType(minussym))
		advanceToken();
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