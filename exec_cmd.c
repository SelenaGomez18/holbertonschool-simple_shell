#include "main.h"

int execute_command(char *command, char **args, char **env)
{
	pid_t pid;
	int status;
	char *cmd_path;

	cmd_path = find_full_path(command);

	if (!cmd_path)
	{
		write(STDERR_FILENO, command, strlen(command));
		write(STDERR_FILENO, ": command not found\n", 21);
		return (-1);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		return (-1);
	}

	if (pid == 0)
	{
		if (execve(cmd_path, args, env) == -1)
		{
			perror(command);
			free(cmd_path);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
	}

	free(cmd_path);
	return (0);
}
