#include "headers/parser.h"

void program(Token **tokenList)
{
	if (tokenList == NULL)
	{
		printf("TOKENLIST IS NULL\n");
		exit(0);
	}
	tokenIndex = -1;
	rfIndex = 0;
	codeIndex = 0;
	// Addresses 0, 1, 2, 3 are reserved for FV, SL, DL, RA
	address = 4;
	tokens = tokenList;

	advanceToken();
	block(0, 0);
	if (!ensureType(periodsym))
		error(9);

	// table[1].level = -1;
	// table[1].address = -1;
	emit(SIO3, 0, 0, 3);
}

void block(int lev, int tx)
{
	if (lev > MAX_LEXI_LEVELS)
		error(28);

	//int prev_sp = sp;

	int dx, tx0, cx0;
	dx = 4;
	tx0 = tx;
	table[tx].address = codeIndex;
	emit(JMP, 0, 0, codeIndex);

	if (ensureType(constsym))
	{
		constDecl(lev, &tx, &dx);
	}
	if (ensureType(varsym))
	{
		varDecl(lev, &tx, &dx);
	}

	while (ensureType(procsym))
	{
		procDecl(lev, &tx, &dx);
	}
	code[table[tx0].address].m = codeIndex;
	table[tx0].address = codeIndex;
	cx0 = codeIndex;
	emit(INC, 0, 0, dx); 

	statement(lev, &tx);

	if (ensureType(periodsym))
		return;

	emit(RTN, 0, 0, 0);
	//sp = prev_sp;
}

void constDecl(int lev, int *ptx, int *pdx)
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
		// Common error catch
		if (ensureType(becomessym))
			error(1);
		if (!ensureType(eqlsym))
			error(3);
		advanceToken();
		if (!ensureType(numbersym))
			error(2);
		val = token->number;
		advanceToken();

		// Enter const into symbol table
		enter(1, ident, val, ptx, pdx, lev);

		// Store the const in the stack (DONT)
		// emit(LIT, 0, 0, val);
		// emit(STO, 0, 0, address - 1);

	}
	while (ensureType(commasym));

	if (!ensureType(semicolonsym))
		error(17);
	advanceToken();

	free(ident);
}

void varDecl(int lev, int *ptx, int *pdx)
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
		enter(2, ident, 0, ptx, pdx, lev);
	}
	while(ensureType(commasym));

	// Token was advanced, but is still an identifier
	if (ensureType(identsym) || isReserved(token->identifier) > 0)
		error(5);
	if (!ensureType(semicolonsym))
		error(17);
	advanceToken();

	free(ident);
}

void procDecl(int lev, int *ptx, int *pdx)
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

		if (!ensureType(semicolonsym))
			error(17);
		advanceToken();

		address = 4;
		// Enter procedure into symbol table
		enter(3, ident, 0, ptx, pdx, lev);
		block(lev + 1, *ptx);
		if (!ensureType(semicolonsym))
			error(17);

		advanceToken();
	}
	while (ensureType(procsym));
	free(ident);
}

void statement(int lev, int *ptx)
{
	int i, cx1, cx2;

	if (ensureType(identsym))
	{
		i = lookup(token->identifier, ptx);
		if (i == 0)
			error(11);
		if (table[i].kind != identsym)
			error(12);

		advanceToken();
		if (!ensureType(becomessym))
			error(13);
		advanceToken();
		expression(lev, ptx);

		emit(STO, 0, lev - table[i].level, table[i].address);
	}
	else if (ensureType(callsym))
	{
		advanceToken();

		if (!ensureType(identsym))
			error(14);

		int i = lookup(token->identifier, ptx);
		if (i == 0)
			error(11);
		if (table[i].kind != 3)
			error(15);

		emit(CAL, 0, lev - table[i].level, table[i].address);

		advanceToken();
	}
	else if (ensureType(beginsym))
	{
		advanceToken();
		// begin found but no statement
		if (ensureType(endsym))
			error(7);
		statement(lev, ptx);

		// Catch statements with no semicolon separation
		if (!ensureType(semicolonsym) && !ensureType(endsym))
			error(10);
		while (ensureType(semicolonsym))
		{
			advanceToken();
			// Cannot have 'end' following a semicolon in this grammar
			// if (ensureType(endsym))
			// 	error(19);
			statement(lev, ptx);
		}
		if (!ensureType(endsym))
			error(26);
		advanceToken();
	}
	else if (ensureType(ifsym))
	{
		advanceToken();
		condition(lev, ptx);

		if (!ensureType(thensym))
			error(16);
		advanceToken();
		cx1 = codeIndex;
		emit(JPC, 0, 0, 0);
		statement(lev, ptx);
		if (ensureType(elsesym))
		{
			advanceToken();
			code[cx1].m = codeIndex + 1;
			cx1 = codeIndex;
			emit(JMP, 0, 0, cx1);
			statement(lev, ptx);
		}
		code[cx1].m = codeIndex;
	}
	else if (ensureType(whilesym))
	{
		cx1 = codeIndex;
		advanceToken();
		condition(lev, ptx);
		cx2 = codeIndex;
		emit(JPC, 0, 0, 0);
		if (!ensureType(dosym))
			error(18);
		advanceToken();
		statement(lev, ptx);
		emit(JMP, 0, 0, cx1);
		code[cx2].m = codeIndex;
	}
	else if (ensureType(readsym))
	{
		advanceToken();

		emit(SIO2, rfIndex, 0, 2);
		if (!ensureType(identsym))
			error(27);
		i = lookup(token->identifier, ptx);
		if (i == 0)
			error(11);
		else if (table[i].kind != 2)
			error(12);
		emit(STO, rfIndex, lev - table[i].level, table[i].address);

		advanceToken();
	}
	else if(ensureType(writesym))
	{
		advanceToken();
		expression(lev, ptx);

		emit(SIO1, rfIndex, 0, 1);
	}
}

void condition(int lev, int *ptx)
{
	if (ensureType(oddsym))
	{
		advanceToken();
		expression(lev, ptx);
		emit(ODD, rfIndex, rfIndex, 0);
	}
	else
	{
		expression(lev, ptx);
		// retrieved last val found

		if (!isRelop())
			error(20);
		int relop = token->type;

		advanceToken();
		rfIndex += 1;
		expression(lev, ptx);
		relOp(relop);
	}
}

void relOp(TokenType type)
{
	TokenType opType = type;
	switch(opType)
	{
		case eqlsym:
			emit(EQL, 0, 0, 1);
			break;
		case neqsym:
			emit(NEQ, 0, 0, 1);
			break;
		case lessym:
			emit(LSS, 0, 0, 1);
			break;
		case leqsym:
			emit(LEQ, 0, 0, 1);
			break;
		case gtrsym:
			emit(GTR, 0, 0, 1);
			break;
		case geqsym:
			emit(GEQ, 0, 0, 1);
			break;
		default:
			break;
	}
}

int isRelop(void)
{
	if (token == NULL)
		return 0;
	TokenType opType = token->type;
	switch(opType)
	{
		case eqlsym:
			return 1;
		case neqsym:
			return 1;
		case lessym:
			return 1;
		case leqsym:
			return 1;
		case gtrsym:
			return 1;
		case geqsym:
			return 1;
		default:
			break;
	}
	return 0;
}

void expression(int lev, int *ptx)
{
	if (ensureType(plussym) || ensureType(minussym))
	{
		int addop = token->type;
		advanceToken();
		term(lev, ptx);
		if (addop == minussym)
			emit(NEG, 0, 1, 0);
	}

	term(lev, ptx);

	while (ensureType(plussym) || ensureType(minussym))
	{
		int addop = token->type;
		advanceToken();
		term(lev, ptx);
		if (addop == plussym)
			emit(ADD, 0, 0, 1);
		else
			emit(SUB, 0, 0, 1);
		rfIndex -= 1;
	}
	// Be careful with this here when it comes to ((())) nested expressions
	rfIndex = 0;
}

void term(int lev, int *ptx)
{
	factor(lev, ptx);

	while (ensureType(multsym) || ensureType(slashsym))
	{
		int mulop = token->type;
		advanceToken();
		factor(lev, ptx);
		if (mulop == multsym)
			emit(MUL, 0, 0, 1);
		else
			emit(DIV, 0, 0, 1);
		rfIndex -= 1;
	}
}

void factor(int lev, int *ptx)
{
	int kind, i, level, adr, val;

	if (ensureType(identsym))
	{
		// lookup in symbol table, load found value into register
		// if symbol exists
		i = lookup(token->identifier, ptx);
		if (i == 0)
			error(11);
		kind = table[i].kind;
		level = table[i].level;
		adr = table[i].address;
		val = table[i].val;
		// const
		if (table[i].kind == 1)
			emit(LIT, rfIndex, 0, table[i].val);
		// var
		else if (table[i].kind == 2)
			emit(LOD, rfIndex, lev - table[i].level, table[i].address);

		rfIndex += 1;
		advanceToken();
	}
	else if (ensureType(numbersym))
	{
		// load literal value into register
		emit(LIT, rfIndex, 0, token->number);
		rfIndex += 1;
		advanceToken();
	}
	else if (ensureType(lparentsym))
	{
		advanceToken();
		expression(lev, ptx);
		if (!ensureType(rparentsym))
			error(22);
		advanceToken();
	}
	else
		error(27);
}
