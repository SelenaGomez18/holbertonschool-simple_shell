#include "main.h"

int execute_command(char *command, char **args, char **env)
{
	if (strchr(command, '/'))
	{
		execve(command, args, env);
		perror(command);
		return -1;
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
		return -1;
	}
}
