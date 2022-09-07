#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "main.h"
#define ps(x) (printf("%s\n", (x)))
#define pd(x) (printf("%d\n", (x)))
#define plu(x) (printf("%lu\n", (x)))


/**
 * print_alias - prints an alias name=value entry..
 * @sarr: the array of strings containing command-line arguments to execute.
 * @alias: address of the array of strings representing the shell's aliases.
 *
 * Return: always 1 for the shell's loop to continue.
 */
int print_alias(char **sarr, char ***alias)
{
	int i, j, flag = 0, len;

	len = _strlen(sarr[0]);
	for (i = 0; (*alias)[i]; i++)
	{
		for (j = 0; j < len; j++)
		{
			if (sarr[0][j] == (*alias)[i][j])
			{
				if (((*alias)[i][j + 1] == '='))
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
			fprintf2(STDOUT_FILENO, "alias %s\n", (*alias)[i]);
			return (1);
		}
	}
	/* if name is not found */
	fprintf2(STDERR_FILENO, "Name not found\n"); /* print to stderr */
	return (1);
}
