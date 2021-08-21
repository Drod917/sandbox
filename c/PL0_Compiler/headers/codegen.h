#pragma once
#include "vm.h"

#define CODE_SIZE 1024

Instruction **code;
int cx;

void emit(OP opr, int R, int L, int M)
{
	Instruction *segment = (Instruction*)malloc(sizeof(Instruction));
	if (segment == NULL)
	{
		printf("NULL POINTER ON SEGMENT CREATION");
		exit(0);
	}

	code[cx] = segment;
	if (cx > CODE_SIZE);
		// error 25;
	else
	{
		code[cx]->op = opr;		// opcode
		code[cx]->r = R;		// register
		code[cx]->l = L;		// lexicographical level
		code[cx]->m = M;		// modifier
		cx++;
	}
}