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

int char_srch2(char *str, char xter);


/**
 * alias2 - manages the shell's alias emvironment via user commands..
 * @sarr: array of strings containing command-line input.
 * @envp: address of environment.
 * @status: address of status int.
 * @_free: address of int indicating whether, or not, to free envp.
 *
 * Return: always 0.
 */
int alias2(char **sarr, char ***envp __attribute__((unused)),
		int *status __attribute__((unused)), int *_free)
{
	int i, equ, r;
	char **sarr2;

	(void)_free;
	if (!sarr[1])
	{
		for (i = 0; shstruct(NULL)->alias[i]; i++)
		{
			fprintf2(STDOUT_FILENO, "%s\n", shstruct(NULL)->alias[i]);
		}
		return (1);
	}
	for (i = 1; sarr[i]; i++)
	{
		equ = char_srch2(sarr[i], '='); /* see whether = is in sarr[i] before... */
		sarr2 = str_arr(sarr[i], "=\n"); /* ...tokenization */
		switch (equ)
		{
			case 1:
				r = upd_alias_val(sarr2, &shstruct(NULL)->alias);
				free(sarr2);
				break;
			case 2:
				r = upd_alias_noval(sarr2, &shstruct(NULL)->alias);
				free(sarr2);
				break;
			case 3:
				r = print_alias(sarr2, &shstruct(NULL)->alias);
				free(sarr2);
		}
	}

	return (r);
}




/**
 * char_srch2 - searches for the presence of '=' in str.
 * @str: string to search in.
 * @xter: character to look for in str.
 *
 * Return: 1 if xter is found and next xter is non-null;
 * 2, if xter is found and next xter is null byte; and 3 if '=' is not present.
 */
int char_srch2(char *str, char xter)
{
	int i;

	for (i = 0; str[i]; i++)
	{
		if (str[i] == xter)
		{
			if (str[++i] == 0)
				return (2);
			else
				return (1);
		}
	}

	return (3);
}
