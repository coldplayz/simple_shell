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

#define ps(x) (printf("%s\n", (x)))
#define pd(x) (printf("%d\n", (x)))

/**
 * char_srchANY - searches for the presence of xter in str.
 * @str: string to search in.
 * @xter: character to look for in str.
 *
 * Return: 1 if xter is found; 0 otherwise.
 */
int char_srchANY(char *str, char xter)
{
	int i;

	for (i = 0; str[i]; i++)
	{
		if (str[i] == xter)
		{
			return (i);
		}
	}

	return (0);
}
