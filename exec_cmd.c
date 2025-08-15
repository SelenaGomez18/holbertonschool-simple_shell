#include "main.h"

/**
 * get_command_path - Finds the full path of a command
 * @command: The command to find
 * @shell_name: Shell name (argv[0])
 * @count: Command counter
 * @last_status: Pointer to last command status
 *
 * Return: Pointer to command path or NULL if not found
 */
char *get_command_path(char *command, char *shell_name,
		int count, int *last_status)
{
	char *cmd_path = find_full_path(command);

	if (!cmd_path)
	{
		fprintf(stderr, "%s: %d: %s: not found\n",
			shell_name, count, command);
		*last_status = 127;
	}
	return (cmd_path);
}

/**
 * run_child_process - Executes a command in a child process
 * @cmd_path: Path to the command
 * @args: Arguments for the command
 * @env: Environment variables
 * @command: Original command name
 * @shell_name: Shell name
 * @count: Command counter
 */
void run_child_process(char *cmd_path, char **args, char **env,
		char *command, char *shell_name, int count)
{
	if (execve(cmd_path, args, env) == -1)
	{
		if (errno == ENOENT)
		{
			fprintf(stderr, "%s: %d: %s: not found\n",
				shell_name, count, command);
			free(cmd_path);
			exit(127);
		}
		perror(command);
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
}

/**
 * handle_parent_process - Waits for child and updates status
 * @pid: Process ID of the child
 * @last_status: Pointer to last command status
 */
void handle_parent_process(pid_t pid, int *last_status)
{
	int status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		*last_status = WEXITSTATUS(status);
	else
		*last_status = 1;
}

/**
 * execute_command - Executes a command with arguments and environment
 * @command: The command to execute
 * @args: Arguments for the command
 * @env: Environment variables
 * @shell_name: Name of the shell executable
 * @count: Command counter
 * @last_status: Pointer to last command exit status
 *
 * Return: 0 on success, -1 on failure
 */
int execute_command(char *command, char **args, char **env,
		char *shell_name, int count, int *last_status)
{
	pid_t pid;
	char *cmd_path;

	/* Get the command path */
	cmd_path = get_command_path(command, shell_name, count, last_status);
	if (!cmd_path)
		return (-1);

	/* Create a child process */
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		*last_status = 1;
		return (-1);
	}

	if (pid == 0)
		run_child_process(cmd_path, args, env, command, shell_name, count);
	else
		handle_parent_process(pid, last_status);

	free(cmd_path);
	return (0);
}
