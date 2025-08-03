#include "main.h"

/**
 * main - Loop principal del shell
 * Return: 0 al salir
 */
int main(void)
{
    char **args;
	char *line = NULL, *clean_line;

	while (1)
	{
		display_prompt();
		line = read_line();

		if (line == NULL)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		clean_line = trim_spaces(line);

		if (clean_line[0] != '\0')
		{
			args = split_line(clean_line);
			if (args)
			{
				execute_command(args[0], args, environ);
				free (args);
			}
		}

		free(line);
	}

	return (0);
}
