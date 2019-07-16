#include "vm_impl.c"
#include "scanner.c"
#include "parser.c"

int initDriver(int argc, char **argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "PROPER SYNTAX: ./scan <filename>\n");
		exit(0);
	}
	initScanner(argv[1]);
}

int main(int argc, char **argv)
{
	initDriver(argc, argv);

	// Scan source to tokenList and print source program
	tokenList = scan(ifp);
	//printTokenList(tokenList);

	// Parse tokenlist (tokenize the source -> symbol table)
	program(tokenList);
	// No errors if program reaches this point.
	printf("\nNo errors, program is syntactically correct.\n\n");

	// Generate code (symtab -> assembly)
	//printSymbolTable(tableIndex);

	// Convert code to legacy format (double pointer...)
	Instruction **vmCodes = malloc(sizeof(Instruction) * CODE_SIZE);
	int i;
	for (i = 0; i < CODE_SIZE; i++)
	{
		vmCodes[i] = malloc(sizeof(Instruction));
		vmCodes[i]->op = code[i].op;
		vmCodes[i]->r = code[i].r;
		vmCodes[i]->l = code[i].l;
		vmCodes[i]->m = code[i].m;
	}
	// Produce VM output (run assembly in VM)
	vm(vmCodes, codeIndex);
	return 0;
}
