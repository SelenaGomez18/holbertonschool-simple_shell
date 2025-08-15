#include "main.h"

/**
 * free_args - Frees a NULL-terminated array of strings
 * @args: Pointer to the array of strings
 *
 * Description: This function frees each string in the array
 * and then frees the array itself. If args is NULL, it does nothing.
 */
void free_args(char **args)
{
	int i;

	if (!args)
		return;

	for (i = 0; args[i]; i++)
		free(args[i]);

	free(args);
}
