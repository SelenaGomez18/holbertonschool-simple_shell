#include "shell.h"

/**
 * prompt - display the prompt if input is interactive
 * @ps1: prompt string
 *
 * Return: void
 */
void prompt(const char *ps1)
{
	if (isatty(STDIN_FILENO))
	{
		if (ps1)
			write(STDOUT_FILENO, ps1, strlen(ps1));
	}
}

/**
 * handle_line - process a single input line: tokenize, builtin or execute
 * @line: input line without trailing '\n' (modifiable)
 * @prog_name: program name (argv[0]) for error messages
 * @line_number: pointer to current line number (for error messages)
 *
 * Return: resulting exit status (0-255), or -1 to indicate exit requested
 */
int handle_line(char *line, char *prog_name, unsigned int *line_number)
{
	char **args;
	int status = 0;
	int ib;

	if (line == NULL)
		return (0);

	(*line_number)++;

	/* empty line -> nothing to do */
	if (line[0] == '\0')
		return (0);

	args = split_line(line);
	if (args == NULL)
		return (0);

	if (args[0] == NULL)
	{
		free_tokens(args);
		return (0);
	}

	/* check builtin: 0 not builtin, 1 builtin ok, -1 builtin requested exit */
	ib = is_builtin(args, prog_name, line_number);
	if (ib == -1)
	{
		free_tokens(args);
		return (-1);
	}
	if (ib == 1)
	{
		free_tokens(args);
		return (0);
	}

	/* external command */
	status = execute_cmd(args, prog_name, line_number);
	free_tokens(args);
	return (status & 0xFF);
}

/**
 * main - entry point for the shell
 * @argc: number of arguments
 * @argv: argument vector
 *
 * Return: last observed command exit status (0-255)
 * or last_status if exit builtin used
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	unsigned int line_number = 0;
	int last_status = 0;
	int res;

	(void)argc;

	while (1)
	{
		prompt("($) ");
		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			free(line);
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			return (last_status & 0xFF);
		}

		/* remove trailing newline */
		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		res = handle_line(line, argv[0], &line_number);

		/* if builtin requested exit, cleanup and exit with last_status */
		if (res == -1)
		{
			free(line);
			return (last_status & 0xFF);
		}

		last_status = res & 0xFF;
	}

	/* unreachable, but keep a consistent return */
	return (last_status & 0xFF);
}
