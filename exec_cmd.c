#include "main.h"

int execute_command(char *command, char **args, char **env)
{
	pid_t pid;
	int status;
	char *path_env, *path_copy, *token;
	char full_path[1024];

	if (strchr(command, '/') != NULL)
	{
		if (access(command, X_OK) != 0)
		{
			perror(command);
			return (-1);
		}
		pid = fork();
		if (pid == 0)
		{
			execve(command, args, env);
			perror(command);
			exit(EXIT_FAILURE);
		}
		else if (pid > 0)
			waitpid(pid, &status, 0);
		else
			perror("fork");
		return (0);
	}

	path_env = getenv("PATH");
	if (!path_env)
		return (-1);

	path_copy = strdup(path_env);
	if (!path_copy)
		return (-1);

	token = strtok(path_copy, ":");
	while (token)
	{
		snprintf(full_path, sizeof(full_path), "%s/%s", token, command);
		if (access(full_path, X_OK) == 0)
		{
			pid = fork();
			if (pid == 0)
			{
				execve(full_path, args, env);
				perror(command);
				exit(EXIT_FAILURE);
			}
			else if (pid > 0)
			{
				waitpid(pid, &status, 0);
			}
			else
			{
				perror("fork");
			}
			free(path_copy);
			return (0);
		}
		token = strtok(NULL, ":");
	}

	fprintf(stderr, "%s: command not found\n", command);
	free(path_copy);
	return (-1);
}
