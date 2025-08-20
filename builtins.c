#include "shell.h"

/**
 * print_line - write a string followed by '\n' using write
 * @s: string to print (must not be NULL)
 *
 * Return: void
 */
static void print_line(const char *s)
{
	size_t len = 0;

	if (s == NULL)
		return;
	while (s[len])
		len++;
	write(STDOUT_FILENO, s, len);
	write(STDOUT_FILENO, "\n", 1);
}

/**
 * strip_cr - remove trailing CR ('\r') if present
 * @s: modifiable string
 *
 * Return: void
 */
static void strip_cr(char *s)
{
	size_t len;

	if (s == NULL)
		return;
	len = strlen(s);
	if (len > 0 && s[len - 1] == '\r')
		s[len - 1] = '\0';
}

/**
 * builtin_env - print the current environment, one entry per line
 * @args: not used
 *
 * Return: 0 on success
 */
int builtin_env(char **args)
{
	char **env;

	(void)args;
	env = environ;
	if (env == NULL)
		return (0);

	while (*env)
	{
		print_line(*env);
		env++;
	}

	return (0);
}

/**
 * builtin_exit - request shell termination
 * @args: not used
 *
 * Instead of exiting directly, return -1 to indicate to the caller
 * that the shell should terminate cleanly.
 *
 * Return: -1 to indicate exit requested
 */
int builtin_exit(char **args)
{
	(void)args;
	return (-1);
}

/**
 * is_builtin - check for and execute builtin commands
 * @args: argument vector (argv)
 * @prog_name: program name (argv[0]) for error messages (unused)
 * @line_number: pointer to the current line number (unused)
 *
 * Return: 1 if a builtin was executed (normal),
 *         -1 if builtin requested exit,
 *         0 if not a builtin
 */
int is_builtin(char **args, char *prog_name, unsigned int *line_number)
{
	int ret;

	(void)prog_name;
	(void)line_number;

	if (args == NULL || args[0] == NULL)
		return (0);

	strip_cr(args[0]);

	if (strcmp(args[0], "exit") == 0)
	{
		ret = builtin_exit(args);
		if (ret == -1)
			return (-1);
		return (1);
	}
	if (strcmp(args[0], "env") == 0)
	{
		builtin_env(args);
		return (1);
	}

	return (0);
}
