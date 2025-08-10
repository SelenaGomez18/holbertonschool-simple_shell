#include "main.h"

/**
 * execute_command - Executes a command with arguments and environment.
 * @program_name: Name of the shell executable (argv[0]).
 * @count: Command counter.
 * @command: The command to execute.
 * @args: Arguments for the command.
 * @env: Environment variables.
 *
 * Return: 0 on success, -1 on failure.
 */
int execute_command(char *command, char **args, char **env, char *shell_name, int count, int *last_status)
{
	pid_t pid;
	int status;
	char *cmd_path;

	cmd_path = find_full_path(command);

	if (!cmd_path)
	{
		fprintf(stderr, "%s: %d: %s: not found\n", shell_name, count, command);
		*last_status = 127;
		return (-1);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		*last_status = 1;
		return (-1);
	}

	if (pid == 0)
	{
		if (execve(cmd_path, args, env) == -1)
		{
			if (errno == ENOENT)
			{
				fprintf(stderr, "%s: %d: %s: not found\n", shell_name, count, command);
				free(cmd_path);
				exit(127);
			}
			perror(command);
			free(cmd_path);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			*last_status = WEXITSTATUS(status);
		else
			*last_status = 1;
	}

	free(cmd_path);
	return (0);
}
