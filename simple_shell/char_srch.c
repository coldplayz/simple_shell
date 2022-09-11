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
 * char_srch - searches for the presence of xter in str.
 * @str: string to search in.
 * @xter: character to look for in str.
 *
 * Return: 1 if xter is found; 0 otherwise.
 */
int char_srch(char *str, char xter)
{
	int i = 0;

	switch (xter)
	{
		case ';':
			for (i = 0; str[i]; i++)
			{
				if (str[i] == xter)
					return (1);
			}
			break;
		case '&':
			for (i = 0; str[i]; i++)
			{
				if (str[i] == xter)
				{
					if (str[++i] == xter)
						return (1);
					else
						return (0);
				}
			}
			break;
		case '|':
			for (i = 0; str[i]; i++)
			{
				if (str[i] == xter)
				{
					if (str[++i] == xter)
						return (1);
					else
						return (0);
				}
			}
	}

	return (0);
}
