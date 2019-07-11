#include "vm_impl.c"

#define CODE_SIZE 1024

Instruction *codeSegment;
Instruction **code;
int cx;

void emit(int op, int R, int L, int M)
{
	if (cx > CODE_SIZE);
		// error 25;
	else
	{
		code[cx]->op = op;	// opcode
		code[cx]->r = R;		// register
		code[cx]->l = L;		// lexicographical level
		code[cx]->m = M;		// modifier
		cx++;
	}
}