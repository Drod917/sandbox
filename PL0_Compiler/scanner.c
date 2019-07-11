// Daniel Rodriguez
// Professor Montagne
// COP 3402 Spring 2019 (NID da268008)

#include "scanner.h"

Token **scan(FILE *ifp)
{
	char c;
	Token **tokenList = allocTokenList();

	c = fgetc(ifp);

	while (1)
	{
		char buffer[MAX_IDENT_LENGTH];
		int bufferIndex = 0;

		if (tokenIndex > MAX_TOKENS)
		{
			fprintf(stderr, "MAX TOKEN OVERFLOW\n");
			exit(0);
		}

		if (isalpha(c) || c == '_')
		{
			// uncomment to allow lexeme length overflow, splitting up tokens
			// instead of crashing.
			while (isalpha(c) || //&& bufferIndex < MAX_IDENT_LENGTH - 1) ||
					c == '_' || //&& bufferIndex < MAX_IDENT_LENGTH - 1) ||
					isdigit(c)) //&& bufferIndex < MAX_IDENT_LENGTH - 1))
			{

				buffer[bufferIndex++] = c;
				c = fgetc(ifp);
			}

			buffer[bufferIndex++] = '\0';
			int reservedType = isReserved(buffer);

			if (reservedType != -1)
			{
				tokenList[tokenIndex++] = newToken(reservedType);
			}
			else
				tokenList[tokenIndex++] = newIdent(identsym, buffer);
		}
		else if (isdigit(c))
		{
			while (isdigit(c) )//&& bufferIndex < MAX_IDENT_LENGTH - 1)
			{
				buffer[bufferIndex++] = c;
				c = fgetc(ifp);
				if (isalpha(c))
				{
					fprintf(stderr, "A token's name begins with a number! Exiting...\n");
					exit(0);
				}
			}

			buffer[bufferIndex] = '\0';
			tokenList[tokenIndex++] = newNumber(numbersym, atoi(buffer));
		}
		else if (ispunct(c))
		{
			// // comment check
			// if ('/' == c)
			// {
			// 	c = fgetc(ifp);
			// 	if (c != '*')
			// 	{
			// 		fprintf(stderr, "PROPER COMMENT SYNTAX: /* <comment> */\n");
			// 		exit(0);
			// 	}
			// 	// c is *
			// 	else
			// 	{
			// 		c = fgetc(ifp);
			// 		while (c != '*')
			// 		{
			// 			c = fgetc(ifp);
			// 		}

			// 		c = fgetc(ifp);

			// 		if (c != '/')
			// 		{
			// 			fprintf(stderr, "PROPER COMMENT SYNTAX: /* <comment> */\n");
			// 			exit(0);
			// 		}
			// 	}
			// }
			if (c == ':')
			{
				c = fgetc(ifp);
				if (c == '=')
				{
					tokenList[tokenIndex++] = newToken(becomessym);
					c = fgetc(ifp);
				}
			}
			else if (c == '+')
			{
				tokenList[tokenIndex++] = newToken(plussym);
				c = fgetc(ifp);
			}
			else if (c == '-')
			{
				tokenList[tokenIndex++] = newToken(minussym);
				c = fgetc(ifp);
			}
			else if (c == '*')
			{
				tokenList[tokenIndex++] = newToken(multsym);
				c = fgetc(ifp);
			}
			else if (c == '/')
			{
				tokenList[tokenIndex++] = newToken(slashsym);
				c = fgetc(ifp);
			}
			else if (c == '%')
			{
				tokenList[tokenIndex++] = newToken(oddsym);
				c = fgetc(ifp);
			}
			else if (c == '(')
			{
				tokenList[tokenIndex++] = newToken(lparentsym);
				c = fgetc(ifp);
			}
			else if (c == ')')
			{
				tokenList[tokenIndex++] = newToken(rparentsym);
				c = fgetc(ifp);
			}
			else if (c == '<')
			{
				c = fgetc(ifp);
				if (c == '=')
				{
					tokenList[tokenIndex++] = newToken(leqsym);
					c = fgetc(ifp);
				}
				else if (c == '>')
				{
					tokenList[tokenIndex++] = newToken(neqsym);
					c = fgetc(ifp);
				}
				else
					tokenList[tokenIndex++] = newToken(lessym);
			}
			else if (c == '>')
			{
				c = fgetc(ifp);
				if (c == '=')
				{
					tokenList[tokenIndex++] = newToken(geqsym);
					c = fgetc(ifp);
				}
				else
					tokenList[tokenIndex++] = newToken(gtrsym);
			}
			else if (c == '=')
			{
				tokenList[tokenIndex++] = newToken(eqlsym);
				c = fgetc(ifp);
			}
			else if (c == ',')
			{
				tokenList[tokenIndex++] = newToken(commasym);
				c = fgetc(ifp);
			}
			else if (c == ';')
			{
				tokenList[tokenIndex++] = newToken(semicolonsym);
				c = fgetc(ifp);
			}
			else if (c == '.')
			{
				tokenList[tokenIndex++] = newToken(periodsym);
				c = fgetc(ifp);
			}
			else
			{
				fprintf(stderr, "TOKEN  %c NOT FOUND\n", c);
				exit(0);
			}
		}
		else if (isspace(c))
			c = fgetc(ifp);
		else if (c == EOF)
		{
			if (debug)
			{
				fprintf(stdout,"\n...End of file reached...\n\n");
			}
			break;
		}
	}

	return tokenList;
}

void initScanner(char *filename)
{
	ifp = fopen(filename, "r");
	if (ifp == NULL)
	{
		fprintf(stderr, "FILE %s NOT FOUND\n", filename);
		exit(0);
	}

	char c;
	fprintf(stdout, "\nSource program:\n\n");

	// As of raspbian stretch, fgetc() returns 255 instead of expected -1
	// use -fsigned-char when compiling with gcc to avoid infinite looping
	while (c != EOF)
	{
		fprintf(stdout, "%c", c);
		c = fgetc(ifp);
	}
	fprintf(stdout, "\n");

	fclose(ifp);
	ifp = fopen(filename, "r");
	if (ifp == NULL)
	{
		fprintf(stderr, "FILE %s NOT FOUND\n", filename);
		exit(0);
	}

}

// int main(int argc, char **argv)
// {
// 	if (argc < 2)
// 	{
// 		fprintf(stderr, "PROPER SYNTAX: ./scan <filename>\n");
// 		exit(0);
// 	}

// 	initScanner(argv[1]);

// 	Token **tokenList = scan(ifp);
// 	printTokenList(tokenList);
// 	return 0;
// }
