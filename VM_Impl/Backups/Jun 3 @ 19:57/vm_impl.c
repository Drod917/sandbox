#include "vm_impl.h"


// FUNCTION PROTOTYPES
int base(int l, int base);

// FUNCTION DEFINITIONS
int base(int l, int base)
{
	int b1; //find base L levels down
	b1 = base;
	while (l > 0)
	{
		b1 = base;
	}
}

int main(int argc, char **argv)
{
	// Improper syntax
	if (argc != 2)
	{
		fprintf(stderr, "CORRECT SYNTAX: 'vm <input_file.txt>'\n");
		exit(0);
	}
	// Open the input file, check if it exists
	FILE *ifp = fopen(argv[1], "r");
	if (ifp == NULL)
	{
		fprintf(stderr, "FILENAME '%s' NOT FOUND\n", argv[1]);
		exit(0);
	}
	// Initialize register file & stack
	int i;
	for (i = 0; i < REGISTER_FILE_SIZE; i++)
		RF[i] = 0;
	for (i = 0; i < MAX_STACK_HEIGHT; i++)
		stack[i] = 0;

	/*
	fprintf(stdout, "\t\t\tgp  pc  bp  sp\n");
	fprintf(stdout, "Initial values\t\t%d  %d  %d  %d\n",
			gp, pc, bp, sp);
			*/
	
	// Fetch / Execute / Print cycle
	int lineCount = 0;
	while (!feof(ifp))
	{
		Instruction *instr;
		fprintf(stdout, "%-3d", lineCount++);
		// Fetch
		instr = fetch(ifp);
		// Execute
		execute(instr);
		free(instr);
	}

	// Close the filestream
	fclose(ifp);
	return 0;
}