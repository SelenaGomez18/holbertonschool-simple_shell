#include "main.h"

/**
 * builtin_env - Prints the current environment
 *
 * Return: Always 0
 */
int builtin_env(void)
{
	int i = 0; /* Counter for environment variables */

	/* Loop through each environment variable and print it */
	while (environ[i] != NULL)
	{
		write(STDOUT_FILENO, environ[i], strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}

	return (0);
}
