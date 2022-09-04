#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "main.h"


/**
 * strtok2 - tokenize a string, str, based on delimiters, delim.
 * @str: the string to tokenize.
 * @delim: delimiter characters.
 *
 * Description: works similar to the library's version. See strtok(3).
 * Note: modifies the characters in str. So, not safe.
 * Return: the address of the current extracted token, or NULL.
 */
char *strtok2(char *str, const char *delim)
{
	char *char1;
	static char *ptc;
	int i, j, n = 0, flag = 1;

	if (str != NULL)
		ptc = str;
	if (*ptc == '\0')
		return (NULL);

	if (delim == NULL)
		return (ptc);
	for (i = 0; *ptc; i++)
	{
		for (j = 0; delim[j]; j++)
		{
			if (*ptc == delim[j])
			{
				if (n)
				{
					*ptc = 0;
					ptc++; /* Point to next character for the next call */
					return (char1);
				}
				flag = 0;
			}
		}
		if (flag)
		{
			n++;
			if (n == 1)
				char1 = ptc;
			flag = 1;
		}
		if ((*ptc == '\0') && n) /*A non-delim char was seen by string end, else...*/
			return (char1);
		else if ((*ptc == '\0') && !n) /*none was seen for this call by string end*/
			return (NULL);
		ptc++;
	}
	return (NULL);
}

/* ====================NOTES=================== */

/* L31: n is greater than 0 only if a */
/* non-delimiter character has been encountered. */
/* This makes it possible to ignore any */
/* leading delim xter and prevent a false termination. */

/* L37: flag is set to 0 only when a delim xter is met, */
/* to prevent entrance into the IF block on L41, which is */
/* only meant for when the current value of ptc is a non-delim xter. */
