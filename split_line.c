#include "main.h"

/**
 * split_line - Splits a line into tokens (words) separated by whitespace
 * @line: The input line to split
 *
 * Description: Allocates memory for an array of strings, then uses strtok
 *              to separate the input line into tokens. Each token is
 *              duplicated and stored in the array.
 *
 * Return: Pointer to the array of tokens, or NULL on failure
 */
char **split_line(char *line)
{
	char **tokens = NULL;
	char *token = NULL;
	int i = 0;

	tokens = malloc(sizeof(char *) * 64);
	if (!tokens)
		return (NULL);

	token = strtok(line, " \t\r\n");

	while (token != NULL && i < 63)
	{
		tokens[i] = strdup(token);
		if (!tokens[i])
		{
			free_args(tokens);
			return (NULL);
		}
		i++;
		token = strtok(NULL, " \t\r\n");
	}
	tokens[i] = NULL;

	return (tokens);
}
