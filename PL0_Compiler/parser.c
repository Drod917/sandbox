#include "symtab.h"

// fix the * \n / error in scanner
void advanceToken(void);

void program(Token **tokenList);
void block(void);
void constDecl(void);
void varDecl(void);
void statement(void);
void condition(void);
int isRelop(void);
void expression(void);
void term(void);
void factor(void);

void program(Token **tokenList)
{
	if (tokenList == NULL)
	{
		printf("TOKENLIST IS NULL\n");
		exit(0);
	}
	tokenIndex = -1;
	tableIndex = 1;
	rfIndex = 0;
	codeIndex = 0;
	level = 0;
	// Addresses 0, 1, 2, 3 are reserved for FV, SL, DL, RA
	address = 4;
	tokens = tokenList;

	advanceToken();
	block();
	if (!ensureType(periodsym))
		error(9);
	table[1].level = -1;
	table[1].address = -1;
	emit(SIO3, 0, 0, 3);
}

void block(void)
{
	if (ensureType(constsym))
		constDecl();
	if (ensureType(varsym))
		varDecl();

	statement();
}
void constDecl(void)
{
	char *ident = malloc(sizeof(char) * MAX_IDENT_LENGTH);
	int val;

	do
	{
		advanceToken();
		if (!ensureType(identsym))
			error(4);
		strcpy(ident, token->identifier);

		advanceToken();
		if (!ensureType(eqlsym))
			error(3);
		advanceToken();
		if (!ensureType(numbersym))
			error(2);
		val = token->number;
		advanceToken();

		// Enter const into symbol table
		enter(1, ident, val, level, address);

		// Store the const in the stack
		emit(LIT, 0, 0, val);
		emit(STO, 0, level, address);
	}
	while (ensureType(commasym));

	if (!ensureType(semicolonsym))
		error(17);
	advanceToken();

	free(ident);
}
void varDecl(void)
{
	char *ident = malloc(sizeof(char) * MAX_IDENT_LENGTH);
	if (ident == NULL)
	{
		printf("NULL PTR ON IDENT MALLOC.\n");
		exit(0);
	}

	do
	{
		advanceToken();
		if (!ensureType(identsym))
			error(4);
		strcpy(ident, token->identifier);

		advanceToken();

		// Enter var into symbol table
		enter(2, ident, 0, level, address);
	}
	while(ensureType(commasym));

	if (!ensureType(semicolonsym))
		error(17);
	advanceToken();

	free(ident);
}
void statement(void)
{
	if (ensureType(identsym))
	{
		int i = lookup(token->identifier);
		if (i == 0)
			error(11);
		if (table[i].kind != identsym)
			error(12);

		advanceToken();
		if (!ensureType(becomessym))
			error(13);
		advanceToken();
		expression();

		emit(STO, 0, level, table[i].address);
	}
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
			error(26);
		advanceToken();
	}
	else if (ensureType(ifsym))
	{
		advanceToken();
		condition();

		if (!ensureType(thensym))
			error(16);
		advanceToken();
		statement();
	}
	else if (ensureType(whilesym))
	{
		advanceToken();
		condition();

		if (!ensureType(dosym))
			error(18);
		advanceToken();
		statement();
	}
}
void condition(void)
{
	if (ensureType(oddsym))
	{
		advanceToken();
		expression();
	}
	else
	{
		expression();

		if (!isRelop())
			error(20);
		advanceToken();
		expression();
	}
}

void expression(void)
{
	if (ensureType(plussym) || ensureType(minussym))
	{
		int addop = token->type;
		advanceToken();
		term();
		if (addop == minussym)
			emit(NEG, 0, 1, 0);
	}

	term();

	while (ensureType(plussym) || ensureType(minussym))
	{
		int addop = token->type;
		advanceToken();
		term();
		if (addop == plussym)
			emit(ADD, 0, 0, 1);
		else
			emit(SUB, 0, 0, 1);
	}
	rfIndex = 0;
}
void term(void)
{
	factor();

	while (ensureType(multsym) || ensureType(slashsym))
	{
		int mulop = token->type;
		advanceToken();
		factor();
		if (mulop == multsym)
			emit(MUL, 0, 0, 1);
		else
			emit(DIV, 0, 0, 1);
	}
}
void factor(void)
{
	if (ensureType(identsym))
	{
		// lookup in symbol table, load found value into register
		// if symbol exists
		int i = lookup(token->identifier);
		if (i == 0)
			error(11);

		// const
		if (table[i].kind == 1)
			emit(LIT, rfIndex++, 0, table[i].val);
		// var
		else if (table[i].kind == 2)
			emit(LOD, 0, level, table[i].address);
		advanceToken();
	}
	else if (ensureType(numbersym))
	{
		// load literal value into register
		emit(LIT, 0, 0, token->number);
		advanceToken();
	}
	else if (ensureType(lparentsym))
	{
		advanceToken();
		expression();
		if (!ensureType(rparentsym))
			error(22);
		advanceToken();
	}
	else
		error(27);
}

int isRelop(void)
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