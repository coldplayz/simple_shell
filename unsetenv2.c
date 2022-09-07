#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "main.h"


/**
 * unsetenv2 - unsets/deletes a new environment variable.
 * @sarr: the array containing command-line arguments.
 * @envp: address of the environment.
 * @status: address of int determinig program exit status.
 * @free: address of int indicating whether to free envp (1), or not (0).
 *
 * Return: always 1 for the shell..
 */
int unsetenv2(char **sarr, char ***envp,
		int *status __attribute__((unused)), int *free)
{
	int i, j, flag = 0, len;

	if (!sarr[1])
	{
		fprintf2(STDERR_FILENO, "unsetenv: name NULL\n");
		return (1);
	}

	len = _strlen(sarr[1]);
	for (i = 0; (*envp)[i]; i++)
	{
		for (j = 0; j < len; j++)
		{
			if (sarr[1][j] == (*envp)[i][j])
			{
				if (((*envp)[i][j + 1] == '='))
				{
					/* if xters match */
					flag = 1;
				}
			}
			else
			{
				/* otherwise leave this loop and go to the next env variable */
				flag = 0;
				break;
			}
		}
		if (flag)
		{
			/* if name is found */
			return (del_env(sarr[1], i, envp, free));
		}
	}
	/* if name is not found */
	return (1);
}
