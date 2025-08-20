#include "shell.h"


#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"


/**
* split_line - splits a line into tokens (arguments)
* @line: input line (will be modified)
* Return: NULL on allocation failure, otherwise an array of pointers
*/
char **split_line(char *line)
{
	char **tokens;
	char *token;
	int bufsize = TOK_BUFSIZE;
	int position = 0;


	tokens = malloc(sizeof(char *) * bufsize);
	if (tokens == NULL)
	{
		perror("malloc");
		return (NULL);
	}


	token = strtok(line, " \t\r\n\a");
	while (token != NULL)
	{
		tokens[position++] = strdup(token);
		if (position >= bufsize)
		{
			bufsize += TOK_BUFSIZE;
			tokens = realloc(tokens, sizeof(char *) * bufsize);
			if (tokens == NULL)
			{
				perror("realloc");
				return (NULL);
			}
		}
		token = strtok(NULL, " \t\r\n\a");
	}

	tokens[position] = NULL;
	return (tokens);
}

/**
* free_tokens - frees array created by split line
* @tokens: array of strings
*/
void free_tokens(char **tokens)
{
	int i = 0;

	if (tokens == NULL)
		return;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
