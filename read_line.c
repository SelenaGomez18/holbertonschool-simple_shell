#include "main.h"

/**
 * read_line - Reads a line from standard input
 *
 * Return: Pointer to the string read (dynamically allocated),
 * or NULL on failure or EOF
 *
 * Description: Uses getline() to read input from stdin. Removes
 * the trailing newline character if present.
 */
char *read_line(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	nread = getline(&line, &len, stdin);

	if (nread == -1)
	{
		free(line);
		return (NULL);
	}

	if (line[nread - 1] == '\n')
		line[nread - 1] = '\0';

	return (line);
}
