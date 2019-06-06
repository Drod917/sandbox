// PREPROCESSOR DIRECTIVES
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// CONSTANTS
#define MAX_STACK_HEIGHT 23
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define OP_LENGTH 4
#define REGISTER_FILE_SIZE 8

// FUNCTION PROTOTYPES
struct Instruction *fetch(FILE *ifp);
void execute(struct Instruction *instr);

// STRUCT DEFINITIONS
typedef struct Instruction
{
	int op;	// opcode
	int r;	// R
	int l;	// L
	int m;	// M
}Instruction;

// GLOBAL VARIABLES
int RF[REGISTER_FILE_SIZE], stack[MAX_STACK_HEIGHT];
int bp = 0,
	pc = 0,
	gp = -1,
	sp = MAX_STACK_HEIGHT;

// FUNCTION DEFINITIONS

// Precondition: IFP points to Instr[op] - 1
// Postcondition: IFP points to Instr[m];
Instruction *fetch(FILE *ifp)
{
	if (ifp == NULL)
	{
		fprintf(stderr, "NULL FILE POINTER EXCEPTION\n");
		exit(0);
		//return NULL;
	}

	Instruction *instr = malloc(sizeof(Instruction));
	if (instr == NULL)
		return NULL;

	int op;

	fscanf(ifp, "%d", &op);
	instr->op = op;

	fscanf(ifp, "%d", &op);
	instr->r = op;

	fscanf(ifp, "%d", &op);
	instr->l = op;

	fscanf(ifp, "%d", &op);
	instr->m = op;

	return instr;
}

// Precondition: instr is a valid instruction
// Postcondition: TODO
void execute(Instruction *instr)
{
	if (instr == NULL)
	{
		fprintf(stderr, "INSTRUCTION DOESN'T EXIST\n");
		return;
	}

	// Print & execute the instruction
	char *op = malloc(sizeof(char) * OP_LENGTH);
	if (op == NULL)
		return;

	switch (instr->op)
	{
		// LIT
		case 1:
			strcpy(op, "LIT");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// RTN
		case 2:
			strcpy(op, "RTN");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// LOD
		case 3:
			strcpy(op, "LOD");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// STO
		case 4:
			strcpy(op, "STO");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// CAL
		case 5:
			strcpy(op, "CAL");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// INC
		case 6:
			strcpy(op, "INC");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// JMP
		case 7:
			strcpy(op, "JMP");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			pc = instr->m;


			break;
		// JPC
		case 8:
			strcpy(op, "JPC");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// SIO R, 0, 1
		case 9:
			strcpy(op, "SIO");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// SIO R, 0, 2
		case 10:
			strcpy(op, "SIO");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// SIO R, 0, 3
		case 11:
			strcpy(op, "SIO");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// NEG
		case 12:
			strcpy(op, "NEG");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// ADD
		case 13:
			strcpy(op, "ADD");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// SUB
		case 14:
			strcpy(op, "SUB");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// MUL
		case 15:
			strcpy(op, "MUL");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// DIV
		case 16:
			strcpy(op, "DIV");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// ODD
		case 17:
			strcpy(op, "ODD");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// MOD
		case 18:
			strcpy(op, "MOD");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// EQL
		case 19:
			strcpy(op, "EQL");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// NEQ
		case 20:
			strcpy(op, "NEQ");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// LSS
		case 21:
			strcpy(op, "LSS");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// LEQ
		case 22:
			strcpy(op, "LEQ");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// GTR
		case 23:
			strcpy(op, "GTR");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
		// GEQ
		case 24:
			strcpy(op, "GEQ");
			fprintf(stdout, "%s %d %d %d\n", op, instr->r,
					instr->l, instr->m);
			break;
	}
	free(op);
	return;
}