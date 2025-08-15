#include "main.h"

/**
 * process_line - Process a single input line
 * @line: Input line from user
 * @argv0: Shell name
 * @count: Command counter
 * @last_status: Pointer to last command status
 *
 * Return: 1 to continue loop, 0 to exit shell
 */
int process_line(char *line, char *argv0, int count, int *last_status)
{
	char *clean_line = trim_spaces(line);
	char **args;

	if (clean_line[0] == '#' || handle_variable_assignment(clean_line))
		return (1);

	args = split_line(clean_line);
	if (!args)
		return (1);

	if (handle_echo_status(args, *last_status) ||
		handle_exit_command(args, last_status, line) ||
		handle_env_command(args) ||
		handle_cd_command(args, last_status, line))
	{
		free_args(args);
		return (1);
	}

	count++;
	execute_command(args[0], args, environ, argv0, count, last_status);
	free_args(args);
	return (1);
}

/**
 * main - Entry point of the simple shell program
 * @argc: Argument count (unused)
 * @argv: Argument vector; argv[0] is the shell name
 *
 * Description: Runs an infinite loop that:
 *              - Displays a prompt in interactive mode
 *              - Reads and cleans input lines
 *              - Handles built-in commands (cd, exit, env, echo $?)
 *              - Executes external commands
 *              - Keeps track of the last command status
 *
 * Return: Exit status of the last executed command
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	int count = 0, last_status = 0;

	(void)argc;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			display_prompt();

		line = read_line();
		if (!line)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (!process_line(line, argv[0], count, &last_status))
		{
			free(line);
			break;
		}

		free(line);
	}

	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "OK\n", 3);

	return (last_status);
}
