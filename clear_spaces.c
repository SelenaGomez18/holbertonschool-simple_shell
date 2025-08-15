#include "main.h"
/**
 * trim_spaces - Removes leading and trailing spaces/tabs from a string
 * @str: The string to be trimmed
 *
 * Return: Pointer to the trimmed string
 *
 * Description: This function removes any spaces or tab characters
 * from the beginning and end of the given string. It modifies the
 * string in place and returns a pointer to the new starting position.
 */

char *trim_spaces(char *str)
{
	char *end;

	while (*str == ' ' || *str == '\t')
		str++;

	if (*str == '\0')
		return (str);

	end = str + strlen(str) - 1;

	while (end > str && (*end == ' ' || *end == '\t'))
		end--;

	end[1] = '\0';

	return (str);
}
