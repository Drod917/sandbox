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
// FUNCTION PROTOTYPES
// STRUCT DEFINITIONS
typedef struct Instruction
{
	int op;	// opcode
	int r;	// R
	int l;	// L
	int m;	// M
}Instruction;
//GLOBAL VARS

