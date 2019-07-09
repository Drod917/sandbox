//#include "vm_impl.c"
#include "scanner.c"
#include "parser.c"
//#include "codegen.c"

int initCompiler(int argc, char **argv)
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
	initCompiler(argc, argv);
	// Scan source to tokenList and print source program
	tokenList = scan(ifp);
	printTokenList(tokenList);

	// Parse tokenlist (tokenize the source -> symbol table)
	symbolTable = parse(tokenList);
	printf("No errors, program is syntactically correct.\n");
	// Generate code (symtab -> assembly)
	// Produce VM output (run assembly in VM)

	return 0;
}