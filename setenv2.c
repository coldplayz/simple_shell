#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "main.h"
#define ex (execve("/bin/ls", args, NULL) == (-1))
#define ps(x) (printf("%s\n", (x)))
#define pd(x) (printf("%d\n", (x)))
#define plu(x) (printf("%lu\n", (x)))

/**
 * setenv2 - sets a new environment variable, or updates an existing one.
 * @sarr: the array of strings containing command-line arguments to execute..
 * @envp: the process's environment
 * @status: address of int storing the shell's exit/return status.
 * @free: address of an int indicating
 * whether envp has been previously rellocated.
 *
 * Tests: tested.
 * Return: always 1 for the shell's loop to continue.
 */
int setenv2(char **sarr, char ***envp,
		int *status __attribute__((unused)), int *free)
{
	int i, j, flag = 0, len;

	i = handle_name_val(sarr[1], sarr[2]);
	if (i != 0)
		return (1);
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
			if (_strlen((*envp)[i] + j + 1) >= _strlen(sarr[2]))
				return (edit_env((*envp)[i], sarr[2], (j + 1)));
			else
				return (rellocate_env(sarr[1], sarr[2], i, 1, envp, free));
		}
	}
	/* if name is not found */
	return (rellocate_env(sarr[1], sarr[2], -1, 2, envp, free));
}
