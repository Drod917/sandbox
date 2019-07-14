// Daniel Rodriguez (NID da268008)
// COP3402 (Summer)
// Professor Montagne

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

// STRUCT DEFINITIONS
typedef enum isaOPS
{
	LIT = 1, RTN = 2, LOD = 3, STO = 4, CAL = 5, INC = 6,
	JMP = 7, JPC = 8, SIO1 = 9, SIO2 = 10, SIO3 = 11, NEG = 12,
	ADD = 13, SUB = 14, MUL = 15, DIV = 16, ODD = 17,
	MOD = 18, EQL = 19, NEQ = 20, LSS = 21, LEQ = 22,
	GTR = 23, GEQ = 24
}OP;

typedef struct Instruction
{
	OP op;	// opcode
	int r;	// R
	int l;	// L
	int m;	// M
}Instruction;

// FUNCTION PROTOTYPES
int main(int argc, char **argv);
int base(int l, int base);
void printState();
Instruction **parseInstructions(char *filename);
void printVM(FILE *ifp);
void execute();

// These are the CPU's registers, and memory
int RF[REGISTER_FILE_SIZE], stack[MAX_STACK_HEIGHT];
int running = 1,
	numInstructions = 1,
	bp = 0,
	pc = 0,
	gp = -1,
	sp = MAX_STACK_HEIGHT;
Instruction **instructions;
Instruction *ir;

// FUNCTION DEFINITIONS
// Find base L levels down
int base(int l, int base)
{
	int b1; //find base L levels down
	b1 = base;
	while (l > 0)
	{
		b1 = stack[b1 - 1];	// b1 - 1 points to SL
		l--;
	}
	return b1;
}
void printState(char *op)
{
	fprintf(stdout, "%-4s  %-3d %-3d %-3d", op, ir->r,
			ir->l, ir->m);
	fprintf(stdout, "\t%-3d %-3d %-3d %-3d", gp, pc, bp, sp);
	fprintf(stdout, " ");
	int i;
	for (i = 0; i < MAX_STACK_HEIGHT; i++)
		fprintf(stdout, "%d%s", stack[i], (i == 5) ? " | " : " ");
	fprintf(stdout, "\nRF: ");
	for (i = 0; i < REGISTER_FILE_SIZE; i++)
		fprintf(stdout, "%d%s", RF[i],
			   (i < REGISTER_FILE_SIZE) ? " " : "\n");
	fprintf(stdout, "\n");
}
// Precondition: Instr is a valid instruction
// Postcondition: The IR is loaded with the instruction
void fetch(Instruction *instr)
{
	if (instr == NULL)
	{
		fprintf(stderr, "NULL POINTER EXCEPTION WHILE FETCHING\n");
		exit(0);
		//return;
	}
	ir->op = instr->op;
	ir->r = instr->r;
	ir->l = instr->l;
	ir->m = instr->m;
}
// Precondition: instr is a valid instruction in the IR
// Postcondition: instr is executed, the resulting state is printed
void execute()
{
	// Print & execute the instruction
	char *op = malloc(sizeof(char) * OP_LENGTH);
	if (op == NULL)
		return;
	switch (ir->op)
	{
		// LIT
		case 1:
			// Ex:
			// Print op
			strcpy(op, "lit");
			RF[ir->r] = ir->m;
			// Print state
			printState(op);
			break;
		// RTN
		case 2:
			strcpy(op, "rtn");
			sp = bp + 1;
			bp = stack[sp - 3];
			pc = stack[sp - 4];
			printState(op);
			break;
		// LOD
		case 3:
			strcpy(op, "lod");
			if (base(ir->l, bp) == 0)
				RF[ir->r] = stack[base(ir->l, bp) + ir->m];
			else
				RF[ir->r] = stack[base(ir->l, bp) - ir->m];
			printState(op);
			break;
		// STO
		case 4:
			strcpy(op, "sto");
			if (base(ir->l, bp) == 0)
			{
				stack[base(ir->l, bp) + ir->m] = RF[ir->r];
			}
			else
				stack[base(ir->l, bp) - ir->m] = RF[ir->r];
			printState(op);
			break;
		// CAL
		case 5:
			strcpy(op, "cal");
			if (sp - 4 <= gp)
			{
				fprintf(stderr, "STACK OVERFLOW\n");
                return;
				//exit(0);
			}
			stack[sp - 1] = 0;				 // return val
			stack[sp - 2] = base(ir->l, bp); // static link
			stack[sp - 3] = bp;				 // dynamic link
			stack[sp - 4] = pc;				 // return addr
			bp = sp - 1;
			pc = ir->m;
			printState(op);
			break;
		// INC
		case 6:
			strcpy(op, "inc");
			if (sp - ir->m <= gp)
			{
				fprintf(stderr, "STACK OVERFLOW\n");
				//exit(0);
                return;
			}
			if (bp == 0)
				gp = gp + ir->m;
			else
				sp = sp - ir->m;
			printState(op);
			break;
		// JMP
		case 7:
			strcpy(op, "jmp");
			pc = ir->m;
			printState(op);
			break;
		// JPC
		case 8:
			strcpy(op, "jpc");
			if (RF[ir->r] == 0)
				pc = ir->m;
			printState(op);
			break;
		// SIO R, 0, 1
		case 9:
			strcpy(op, "sio");
			printState(op);
			fprintf(stdout, "\n%d\n", RF[ir->l]);
			break;
		// SIO R, 0, 2
		case 10:
			strcpy(op, "sio");
			scanf("%d", &RF[ir->r]);
			printState(op);
			break;
		// SIO R, 0, 3
		// HALT FLAG
		case 11:
			strcpy(op, "sio");
			printState(op);
			running = 0;
			break;
		// NEG
		case 12:
			strcpy(op, "neg");
			RF[ir->r] = RF[ir->l] - (2 * RF[ir->l]);
			printState(op);
			break;
		// ADD
		case 13:
			strcpy(op, "add");
			RF[ir->r] = RF[ir->l] + RF[ir->m];
			printState(op);
			break;
		// SUB
		case 14:
			strcpy(op, "sub");
			RF[ir->r] = RF[ir->l] - RF[ir->m];
			printState(op);
			break;
		// MUL
		case 15:
			strcpy(op, "mul");
			RF[ir->r] = RF[ir->l] * RF[ir->m];
			printState(op);
			break;
		// DIV
		case 16:
			strcpy(op, "div");
			RF[ir->r] = RF[ir->l] / RF[ir->m];
			printState(op);
			break;
		// ODD
		case 17:
			strcpy(op, "odd");
			RF[ir->r] = RF[ir->r] % 2;
			printState(op);
			break;
		// MOD
		case 18:
			strcpy(op, "mod");
			RF[ir->r] = RF[ir->l] % RF[ir->m];
			printState(op);
			break;
		// EQL
		case 19:
			strcpy(op, "eql");
			RF[ir->r] = (RF[ir->l] == RF[ir->m]);
			printState(op);
			break;
		// NEQ
		case 20:
			strcpy(op, "neq");
			RF[ir->r] = (RF[ir->l] != RF[ir->m]);
			printState(op);
			break;
		// LSS
		case 21:
			strcpy(op, "lss");
			RF[ir->r] = (RF[ir->l] < RF[ir->m]);
			printState(op);
			break;
		// LEQ
		case 22:
			strcpy(op, "leq");
			RF[ir->r] = (RF[ir->l] <= RF[ir->m]);
			printState(op);
			break;
		// GTR
		case 23:
			strcpy(op, "gtr");
			RF[ir->r] = (RF[ir->l] > RF[ir->m]);
			printState(op);
			break;
		// GEQ
		case 24:
			strcpy(op, "geq");
			RF[ir->r] = (RF[ir->l] >= RF[ir->m]);
			printState(op);
			break;
	}
	printf("\n");
	free(op);
	return;
}
// Prints the program instructions in assembly
void printInAssembly()
{
	char *opC = malloc(sizeof(char) * OP_LENGTH);
	if (opC == NULL)
	{
		fprintf(stderr, "Something went wrong!\n");
		exit(0);
	}
	fprintf(stdout, "Line OP   R   L   M\n");
	int i;
	for (i = 0; i < numInstructions; i++)
	{
		switch (instructions[i]->op)
		{
			case 1:
				strcpy(opC, "lit");
				break;
			case 2:
				strcpy(opC, "rtn");
				break;
			case 3:
				strcpy(opC, "lod");
				break;
			case 4:
				strcpy(opC, "sto");
				break;
			case 5:
				strcpy(opC, "cal");
				break;
			case 6:
				strcpy(opC, "inc");
				break;
			case 7:
				strcpy(opC, "jmp");
				break;
			case 8:
				strcpy(opC, "jpc");
				break;
			case 9:
				strcpy(opC, "sio");
				break;
			case 10:
				strcpy(opC, "sio");
				break;
			case 11:
				strcpy(opC, "sio");
				break;
			case 12:
				strcpy(opC, "neg");
				break;
			case 13:
				strcpy(opC, "add");
				break;
			case 14:
				strcpy(opC, "sub");
				break;
			case 15:
				strcpy(opC, "mul");
				break;
			case 16:
				strcpy(opC, "div");
				break;
			case 17:
				strcpy(opC, "odd");
				break;
			case 18:
				strcpy(opC, "mod");
				break;
			case 19:
				strcpy(opC, "eql");
				break;
			case 20:
				strcpy(opC, "neq");
				break;
			case 21:
				strcpy(opC, "lss");
				break;
			case 22:
				strcpy(opC, "leq");
				break;
			case 23:
				strcpy(opC, "gtr");
				break;
			case 24:
				strcpy(opC, "geq");
				break;
		}
		fprintf(stdout, "%-3d %-4s  %-3d %-3d %-3d\n", i, opC,
				instructions[i]->r, instructions[i]->l, instructions[i]->m);
	}
	free(opC);
}

void checkFile(char *filename)
{
	// Open the input file, check if it exists
	FILE *ifp = fopen(filename, "r");
	if (ifp == NULL)
	{
		fprintf(stderr, "FILENAME '%s' NOT FOUND\n", filename);
		exit(0);
	}
	fclose(ifp);
}
void initVM()
{
	ir = malloc(sizeof(Instruction));
	int i;
	for (i = 0; i < REGISTER_FILE_SIZE; i++)
		RF[i] = 0;
	for (i = 0; i < MAX_STACK_HEIGHT; i++)
		stack[i] = 0;

	printInAssembly();

	// Print the initial state of the machine
	fprintf(stdout, "\n\t\t\tgp  pc  bp  sp  data\t      stack\n"
			"Initial values\t\t%d  %d   %d   %d  ", gp, pc, bp, sp);
	for (i = 0; i < MAX_STACK_HEIGHT; i++)
		fprintf(stdout, "%d%s", stack[i], (i == 5) ? " | " : " ");
	fprintf(stdout, "\nRF: ");
	for (i = 0; i < REGISTER_FILE_SIZE; i++)
		fprintf(stdout, "%d%s", RF[i],
			   (i < REGISTER_FILE_SIZE) ? " " : "\n");
	fprintf(stdout, "\n\n");
}

void cleanupVM()
{
	free(ir);
	int i;
	for (i = 0; i < numInstructions; i++)
		free(instructions[i]);
	free(instructions);
}

int vm(Instruction **instructionList, int size)
{
	// Improper syntax detected
	if (instructionList == NULL)
	{
		printf("NULL POINTER PASSED TO VM\n");
		exit(0);
	}
	instructions = instructionList;
	numInstructions = size;

	initVM();
	
	// VM IS READY, BEGIN FETCH / EXECUTE CYCLE
	while (running)
	{
		fprintf(stdout, "%-3d", pc);
		fetch(instructions[pc++]);
		execute(ir);
	}
	cleanupVM();
	return 0;
}
