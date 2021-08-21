// Daniel Rodriguez (NID da268008)
// COP3402 (Summer)
// Professor Montagne
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_STACK_HEIGHT 32
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define OP_LENGTH 4
#define REGISTER_FILE_SIZE 8

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

Instruction **instructions;
Instruction *ir;

int base(int l, int base);
int vm(Instruction **instructionList, int size);
void checkFile(char *filename);
void cleanupVM();
void initVM();
void printState(char *op);
void fetch(Instruction *instr);
void execute();
void printInAssembly();
//void printVM(FILE *ifp);
