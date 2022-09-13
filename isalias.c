#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include "main.h"


/**
 * isalias - checks if a string is an alias name.
 * @name: name string.
 *
 * Return: name's direct/immediate value or NULL.
 */
char *isalias(const char *name)
{
	int i, j, flag = 0, len, len2, len3;

	if (!name)
	{
		return (NULL);
	}

	len = _strlen((char *)name);
	for (i = 0; shstruct(NULL)->alias[i]; i++)
	{
		len2 = _strlen(shstruct(NULL)->alias[i]);

		/* ensures only the minimum length of the two strings to compare is used */
		len3 = (len <= len2) ? len : len2;
		for (j = 0; j < len3; j++)
		{
			if (name[j] == shstruct(NULL)->alias[i][j])
			{
				if ((shstruct(NULL)->alias[i][j + 1] == '=') && (j == len - 1))
				{
					/* if xters match and at end of 'name' */
					flag = 1;
					break;
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
			/* if all specified xters match */
			return (shstruct(NULL)->alias[i] + j + 2);
		}
	}

	return (NULL);
}
