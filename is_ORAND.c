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
 * is_ORAND - searches for the presence of "&&" and/or "||" tokens in str.
 * @str: string to search in.
 *
 * Return: 1 if xter is found; 0 otherwise.
 */
int is_ORAND(char *str)
{
	int i = 0;

	for (i = 0; str[i]; i++)
	{
		if (str[i] == '|')
		{
			if (str[++i] == '|')
			{
				return (1);
			}
			else
			{
				break;
			}
		}
	}

	for (i = 0; str[i]; i++)
	{
		if (str[i] == '&')
		{
			if (str[++i] == '&')
			{
				return (1);
			}
			else
			{
				break;
			}
		}
	}

	return (0);
}
