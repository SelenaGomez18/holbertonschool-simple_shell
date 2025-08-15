#include "main.h"

/**
 * command_has_slash - Checks if a command contains '/'
 * @command: The command to check
 *
 * Return: 1 if contains '/', 0 otherwise
 */
static int command_has_slash(char *command)
{
	return (strchr(command, '/') != NULL);
}

/**
 * check_direct_command - Verifies if a direct path command is executable
 * @command: The command with full or relative path
 *
 * Return: Duplicated string of command if executable, NULL otherwise
 */
static char *check_direct_command(char *command)
{
	if (access(command, X_OK) == 0)
		return (strdup(command));
	return (NULL);
}

/**
 * get_path_variable - Retrieves the PATH environment variable
 *
 * Return: Pointer to PATH value or NULL if not found
 */
static char *get_path_variable(void)
{
	int i = 0;

	while (environ[i])
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
			return (environ[i] + 5);
		i++;
	}
	return (NULL);
}

/**
 * search_command_in_path - Searches for a command in PATH directories
 * @command: Command to find
 * @path_env: PATH value
 *
 * Return: Full path string if found, NULL otherwise
 */
static char *search_command_in_path(char *command, char *path_env)
{
	char *path_copy, *token, *full_path;
	int len;

	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);

	token = strtok(path_copy, ":");
	while (token)
	{
		len = strlen(token) + strlen(command) + 2;
		full_path = malloc(len);
		if (!full_path)
		{
			free(path_copy);
			return (NULL);
		}
		sprintf(full_path, "%s/%s", token, command);

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		token = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

/**
 * find_full_path - Finds the absolute path of a command
 * @command: The command to search
 *
 * Return: Full path string if found, NULL otherwise
 */
char *find_full_path(char *command)
{
	char *path_env;

	if (command_has_slash(command))
		return (check_direct_command(command));

	path_env = get_path_variable();
	if (!path_env || path_env[0] == '\0')
		return (NULL);

	return (search_command_in_path(command, path_env));
}
