#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <dirent.h>
#include "main.h"

#define ex (execve("/bin/ls", args, NULL) == (-1))
#define ps(x) (printf("%s\n", (x)))
#define pd(x) (printf("%d\n", (x)))
#define plu(x) (printf("%lu", (x)))


/**
 * mclTTY2 - handles the shell's command parsing and launching.
 * @line: input string.
 * @envp: address of the process' environment.
 * @bltin_nm: array of names of built-in commands.
 * @b: address of int determining how to free the arrays of command strings.
 * @status: address of int storing exit status.
 * @_free: address of int determining whether or not to free envp.
 * @shell_nm: the shell's pathname.
 *
 * Description: this version of mclTTY is called for space-saparated
 * piped input where the first word is a command, and the others its argument(s).
 * Return: always 1, except in certain cases like when exit() is called, then 0
 */
int mclTTY2(char *shell_nm, int *b, char ***envp,
		int *status, int *_free, char **bltin_nm, char *line)
{
	int a, c = 1;
	char **str_ar2;

	for (a = 0; a < 1; a++) /* run subshell once */
	{
		str_ar2 = in_parser(line, *envp, bltin_nm, b);
		if (!str_ar2) /* command not found */
		{
			shell_nm = shell_nm;
			c = 1;
			continue;
		}
		if (str_ar2[0] == NULL) /* no input - only newline and null character */
		{
			handle_free("d", *b, str_ar2);
			c = 1;
			continue;
		}
		psarr(str_ar2, '+');

		c = launcher(str_ar2, envp, bltin_nm, status, _free);

		handle_free("d", *b, str_ar2);
		*b = 1;
	}
	handle_free("s", 0, line);

	return (c);
}
