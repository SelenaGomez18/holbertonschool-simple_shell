#include "shell.h"

/**
 * print_error_not_found - prints: prog_name: line_num: cmd: not found
 * @prog_name: argv[0]
 * @line_num: line number (for non-interactive scripts)
 * @cmd: command that was not found
 *
 * Return: void
 */
void print_error_not_found(char *prog_name, unsigned int line_num, char *cmd)
{
	char buf[256];

	sprintf(buf, "%s: %u: %s: not found\n", prog_name, line_num, cmd);
	write(STDERR_FILENO, buf, strlen(buf));
}

/**
 * child_exec - attempt to execute cmd_path with args in the child
 * @cmd_path: path to the executable (malloc'd or strdup'd)
 * @args: argument vector (NULL-terminated)
 * @prog_name: program name (for perror)
 *
 * This function does not return on success. On failure it _exit(127).
 */
static void child_exec(char *cmd_path, char **args, char *prog_name)
{
	execve(cmd_path, args, environ);
	/* If execve returns, an error occurred */
	perror(prog_name);
	_exit(127);
}

/**
 * wait_and_get_status - wait for child and obtain a handled exit status
 * @pid: pid of the child process
 *
 * Return: exit code (0-255) or 128+signal if terminated by signal
 */
static int wait_and_get_status(pid_t pid)
{
	int status;

	if (waitpid(pid, &status, 0) == -1)
	{
		/* If waitpid fails, return 1 as a generic error */
		perror("waitpid");
		return (1);
	}

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));

	return (1);
}

/**
 * execute_cmd - find the command path, fork and execve if appropriate
 * @args: argv (NULL-terminated)
 * @prog_name: argv[0] for error messages
 * @line_number: pointer to the line number (for messages)
 *
 * Return: exit status of the command (0-255). Returns 127 if not found.
 */
int execute_cmd(char **args, char *prog_name, unsigned int *line_number)
{
	pid_t pid;
	int exit_status;
	char *cmd_path;

	if (args == NULL || args[0] == NULL)
		return (0);

	/* locate the executable (also handles paths containing '/') */
	cmd_path = find_command_path(args[0]);
	if (cmd_path == NULL)
	{
		print_error_not_found(prog_name, *line_number, args[0]);
		return (127);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		return (1);
	}

	if (pid == 0)
	{
		/* child */
		child_exec(cmd_path, args, prog_name);
		/* not reached */
	}

	/* parent */
	exit_status = wait_and_get_status(pid);
	free(cmd_path);
	return (exit_status);
}
