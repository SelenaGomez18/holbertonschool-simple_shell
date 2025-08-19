#include "shell.h"

/**
 * join_dir_cmd - build a path string "dir/cmd"
 * @dir: directory (not NULL)
 * @cmd: command name (not NULL)
 *
 * Return: malloc'd string with "dir/cmd", or NULL on malloc failure
 */
static char *join_dir_cmd(const char *dir, const char *cmd)
{
	size_t len;
	char *fullpath;

	if (dir == NULL || cmd == NULL)
		return (NULL);

	len = strlen(dir) + 1 + strlen(cmd) + 1;
	fullpath = malloc(len);
	if (fullpath == NULL)
		return (NULL);

	sprintf(fullpath, "%s/%s", dir, cmd);
	return (fullpath);
}

/**
 * path_lookup_in_env - search for cmd inside a PATH value string
 * @path_env: PATH value (e.g. "/bin:/usr/bin")
 * @cmd: command to find
 *
 * Return: malloc'd fullpath if executable found, or NULL if not found
 */
static char *path_lookup_in_env(char *path_env, char *cmd)
{
	char *path_dup, *dir, *fullpath = NULL;

	if (path_env == NULL || cmd == NULL)
		return (NULL);

	path_dup = strdup(path_env);
	if (path_dup == NULL)
		return (NULL);

	dir = strtok(path_dup, ":");
	while (dir != NULL)
	{
		fullpath = join_dir_cmd(dir, cmd);
		if (fullpath == NULL)
		{
			dir = strtok(NULL, ":");
			continue;
		}
		if (access(fullpath, X_OK) == 0)
		{
			free(path_dup);
			return (fullpath);
		}
		free(fullpath);
		fullpath = NULL;
		dir = strtok(NULL, ":");
	}

	free(path_dup);
	return (NULL);
}

/**
 * find_path_value - get the PATH value from environ (without using getenv)
 *
 * Return: pointer to the start of the PATH value
 * inside environ (not malloc'd),
 *         or NULL if PATH is not present
 */
static char *find_path_value(void)
{
	char **env = environ;
	size_t i;

	if (env == NULL)
		return (NULL);

	for (i = 0; env[i] != NULL; i++)
	{
		if (strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
	}

	return (NULL);
}

/**
 * find_command_path - find full path to cmd using PATH
 * or a path containing '/'
 * @cmd: command name
 *
 * Return: malloc'd full path (caller must free) or NULL if not found
 */
char *find_command_path(char *cmd)
{
	char *path_value;

	if (cmd == NULL)
		return (NULL);

	/* If command contains '/', treat it as a path: check executable permission */
	if (strchr(cmd, '/') != NULL)
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}

	/* obtain PATH from environ and search its directories */
	path_value = find_path_value();
	if (path_value == NULL)
		return (NULL);

	return (path_lookup_in_env(path_value, cmd));
}
