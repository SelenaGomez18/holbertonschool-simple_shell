#include "main.h"

/**
 * display_prompt - Displays the shell prompt if in interactive mode
 *
 * Description: This function checks if the shell is running in
 * interactive mode by using isatty on STDIN. If true, it writes
 * the prompt "#cisfun$" to STDOUT.
 */
void display_prompt(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "#cisfun$", 8);
}
