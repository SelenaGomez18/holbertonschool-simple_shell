#include "main.h"

int execute_command(char *command, char **args, char **env)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}

	if (pid == 0)
	{
		if (strchr(command, '/'))
		{
			execve(command, args, env);
			perror(command);
			exit(EXIT_FAILURE);
		}
		else
		{
			char *path = getenv("PATH");
			char *token = strtok(path, ":");
			char full_path[1024];

			while (token != NULL)
			{
				snprintf(full_path, sizeof(full_path), "%s/%s", token, command);
				execve(full_path, args, env);
				token = strtok(NULL, ":");
			}
			perror(command);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
	}

	return (0);
}
