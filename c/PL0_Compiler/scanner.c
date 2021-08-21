// Daniel Rodriguez
// Professor Montagne
// COP 3402 Spring 2019 (NID da268008)
#include "headers/scanner.h"

int tokenIndex;
int debug = 0;

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
			while (isdigit(c))//&& bufferIndex < MAX_IDENT_LENGTH - 1)
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
			// comment check
			if ('/' == c)
			{
				int comments;
				c = fgetc(ifp);

				if (c != '*')
				{
					tokenList[tokenIndex++] = newToken(slashsym);
				}
				else
				{
					c = fgetc(ifp);
					comments = 1;

					while (comments)
					{
						if (c == '*')
						{
							c = fgetc(ifp);
							if (c != '/')
							{
								c = fgetc(ifp);
							}
							else if (c == '/' || c == EOF)
							{
								comments = 0;
								c = fgetc(ifp);
							}
						}
						else
							c = fgetc(ifp);
					}
					// comments = 1;
					// c = fgetc(ifp);
					// while (comments)
					// {
					// 	if (c == '*')
					// 	{
					// 		c = fgetc(ifp);
					// 		if (c == '/')
					// 		{
					// 			comments = 0;
					// 			c = fgetc(ifp);
					// 		}
					// 	}
					// 	else
					// 	{
					// 		c = fgetc(ifp);
					// 	}
					// }
				}
			}	// if comment, should leave with c == token after /
			else if (c == ':')
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
			else if (c == '\n')
			 	c = fgetc(ifp);
			else
			{
				fprintf(stderr, "TOKEN  %c NOT FOUND\n", c);
				exit(0);
			}
		}
		else if (isspace(c))
			c = fgetc(ifp);
		else if (c == '\n')
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

Token *allocToken()
{
	Token *token = malloc(sizeof(Token));
	if (token == NULL)
	{
		fprintf(stderr, "MALLOC FAIL ON NEW TOKEN\n");
		exit(0);
	}
	return token;
}

Token **allocTokenList()
{
	Token **newList = malloc(sizeof(Token *) * MAX_TOKENS);
	if (newList == NULL)
	{		
		fprintf(stderr, "MALLOC FAIL ON NEW TOKENLIST\n");
		exit(0);
	}
}

Token *newToken(int type)
{
	Token *token = allocToken();
	token->type = type;
	token->identifier = getType(type);
	token->number = 0;

	if (debug)
	{
		fprintf(stdout, "Adding reserved token %d...\n", token->type);
	}
	return token;
}

Token *newNumber(int type, int number)
{
	if (number > 99999)
	{
		fprintf(stderr, "Number %d too long!\n", number);
		exit(0);
	}
	
	Token *token = allocToken();
	token->type = type;

	char *numAsIdent = malloc(sizeof(char) * MAX_IDENT_LENGTH);
	token->identifier = NULL;
	token->number = number;

	if (debug)
	{
		fprintf(stdout, "Adding Token %d...\n", token->number);
	}
	return token;
}

Token *newIdent(int type, char *identifier)
{
	if (identifier == NULL)
	{
		fprintf(stderr, "NULL IDENTIFIER ON NEW IDENT\n");
		exit(0);
	}
	Token *token = allocToken();
	token->type = type;
	token->number = 0;
	int identLength = strlen(identifier) + 1; // +1 for windows compilation
	if (identLength > MAX_IDENT_LENGTH)
	{
		fprintf(stderr, "identifier '%s' too long\n", identifier);
		exit(0);
	}
	char *scannedIdent = malloc(sizeof(char) * identLength);
	if (scannedIdent == NULL)
	{
		fprintf(stderr, "MALLOC FAIL ON NEW scannedIdent\n");
		exit(0);
	}
	strncpy(scannedIdent, identifier, identLength);
	token->identifier = scannedIdent;

	if (debug)
	{
		fprintf(stdout, "Adding Token %s...\n", token->identifier);
	}

	return token;
}

int isReserved(char *identifier)
{
	if (identifier == NULL)
	{
		fprintf(stderr, "IDENTIFIER %s IS NULL\n", identifier);
		exit(0);
	}

	if (debug)
	{
		printf("Checking %s...\n\n", identifier);
	}

	if (strcmp(identifier, "odd") == 0)
		return 8;
	else if (strcmp(identifier, "const") == 0)
		return 28;
	else if (strcmp(identifier, "var") == 0)
		return 29;
	else if (strcmp(identifier, "procedure") == 0)
		return 30;
	else if (strcmp(identifier, "begin") == 0)
		return 21;
	else if (strcmp(identifier, "end") == 0)
		return 22;
	else if (strcmp(identifier, "if") == 0)
		return 23;
	else if (strcmp(identifier, "then") == 0)
		return 24;
	else if (strcmp(identifier, "else") == 0)
		return 33;
	else if (strcmp(identifier, "read") == 0)
		return 32;
	else if (strcmp(identifier, "write") == 0)
		return 31;
	else if (strcmp(identifier, "call") == 0)
		return 27;
	else if (strcmp(identifier, "while") == 0)
		return 25;
	else if (strcmp(identifier, "do") == 0)
		return 26;

	return -1;
}

/*
nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5, multsym = 6,
slashsym = 7, oddsym = 8, eqlsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17,
semicolonsym = 18, periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22,
ifsym = 23, thensym = 24, whilesym = 25, dosym = 26, callsym = 27, constsym = 28,
varsym = 29, procsym = 30, writesym = 31, readsym = 32, elsesym = 33	
*/
char *getType(int reservedType)
{
	char *ident = malloc(sizeof(char) * MAX_IDENT_LENGTH);

	switch (reservedType)
	{
		case 4:
			strcpy(ident, "+");
			break;
		case 5:
			strcpy(ident, "-");
			break;
		case 6:
			strcpy(ident, "*");
			break;
		case 7:
			strcpy(ident, "/");
			break;
		case 8:
			strcpy(ident, "odd");
			break;
		case 9:
			strcpy(ident, "=");
			break;
		case 10:
			strcpy(ident, "<>");
			break;
		case 11:
			strcpy(ident, "<");
			break;
		case 12:
			strcpy(ident, "<=");
			break;
		case 13:
			strcpy(ident, ">");
			break;
		case 14:
			strcpy(ident, ">=");
			break;
		case 15:
			strcpy(ident, "(");
			break;
		case 16:
			strcpy(ident, ")");
			break;
		case 17:
			strcpy(ident, ",");
			break;
		case 18:
			strcpy(ident, ";");
			break;
		case 19:
			strcpy(ident, ".");
			break;
		case 20:
			strcpy(ident, ":=");
			break;
		case 21:
			strcpy(ident, "begin");
			break;
		case 22:
			strcpy(ident, "end");
			break;
		case 23:
			strcpy(ident, "if");
			break;
		case 24:
			strcpy(ident, "then");
			break;
		case 25:
			strcpy(ident, "while");
			break;
		case 26:
			strcpy(ident, "do");
			break;
		case 27:
			strcpy(ident, "call");
			break;
		case 28:
			strcpy(ident, "const");
			break;
		case 29:
			strcpy(ident, "var");
			break;
		case 30:
			strcpy(ident, "procedure");
			break;
		case 31:
			strcpy(ident, "write");
			break;
		case 32:
			strcpy(ident, "read");
			break;
		case 33:
			strcpy(ident, "else");
			break;
	}

	return ident;
}

void printTokenList(Token **tokenList)
{
	if (tokenList == NULL)
	{
		fprintf(stderr, "NULL POINTER FOUND TRYING TO PRINT TOKEN LIST\n");
		exit(0);
	}

	fprintf(stdout, "\nLexeme Table:\n\nLexeme\t    Token Type\n");

	// Print Lexeme Table
	int i;
	for (i = 0; i < tokenIndex; i++)
	{
		if (tokenList[i]->identifier == NULL)
		{
			fprintf(stdout, "%-12d%d\n",
					tokenList[i]->number, tokenList[i]->type);

		}
		else
			fprintf(stdout, "%-12s%d\n",
					tokenList[i]->identifier, tokenList[i]->type);
	}

	// Print Lexeme List
	fprintf(stdout, "\nLexeme List:\n");
	for (i = 0; i < tokenIndex; i++)
	{
		// Ident token
		if (tokenList[i]->type == identsym)
		{
			fprintf(stdout, "%d %s%s",
					tokenList[i]->type, tokenList[i]->identifier,
					(i < tokenIndex) ? " " : "\n");
		
		}
		// Number token
		else if (tokenList[i]->type == numbersym)
		{
			fprintf(stdout, "%d %d%s",
					tokenList[i]->type, tokenList[i]->number,
					(i < tokenIndex) ? " " : "\n");
		}
		// Reserved token
		else
			fprintf(stdout, "%d%s",
					tokenList[i]->type,
					(i < tokenIndex) ? " " : "\n");
	}
	fprintf(stdout, "\n");
}