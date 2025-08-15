/**
 * cd_to_home - Change directory to HOME
 * @last_status: Pointer to last command status
 *
 * Return: 0 on success, 1 on failure
 */
int cd_to_home(int *last_status)
{
	char *home = getenv("HOME");

	if (!home)
		home = "/";

	if (chdir(home) != 0)
	{
		perror("cd");
		*last_status = 1;
		return (1);
	}

	*last_status = 0;
	return (0);
}

/**
 * cd_to_path - Change directory to specified path
 * @path: Directory path
 * @last_status: Pointer to last command status
 *
 * Return: 0 on success, 1 on failure
 */
int cd_to_path(char *path, int *last_status)
{
	if (chdir(path) != 0)
	{
		perror("cd");
		*last_status = 1;
		return (1);
	}

	*last_status = 0;
	return (0);
}

/**
 * handle_cd_command - Handles the "cd" built-in command
 * @args: Command arguments
 * @last_status: Pointer to last command status
 * @line: Original input line
 *
 * Return: 1 if cd was executed, 0 otherwise
 */
int handle_cd_command(char **args, int *last_status, char *line)
{
	char old_pwd[PATH_MAX], new_pwd[PATH_MAX];

	if (strcmp(args[0], "cd") != 0)
		return (0);

	if (getcwd(old_pwd, sizeof(old_pwd)) == NULL)
	{
		perror("getcwd");
		*last_status = 1;
		return (1);
	}

	if (!args[1])
		cd_to_home(last_status);
	else
		cd_to_path(args[1], last_status);

	if (getcwd(new_pwd, sizeof(new_pwd)) == NULL)
	{
		perror("getcwd");
		*last_status = 1;
		return (1);
	}

	setenv("OLDPWD", old_pwd, 1);
	setenv("PWD", new_pwd, 1);

	return (1);
}
