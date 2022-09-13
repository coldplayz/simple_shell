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

#define ex (execve("/bin/ls", args, NULL) == (-1))
#define ps(x) (printf("%s\n", (x)))
#define pd(x) (printf("%d\n", (x)))
#define plu(x) (printf("%lu\n", (x)))


/**
 * is_newalias - checks if cmd_name is a
 * non-existent alias that is about to be created.
 * @cmd_name: string that is potentially a new alias.
 *
 * Return: 1 if true; 0 otherwise.
 */
int is_newalias(char *cmd_name)
{
	int i, j = 0, idx = 0;
	char *ptc, **pptc;

	pptc = malloc(sizeof(char *) * str_arrlen(shstruct(NULL)->newalias));
	if (!pptc)
	{
		perror("is_newalias-malloc");
		exit(1);
	}
	for (i = 0; i < (str_arrlen(shstruct(NULL)->newalias)); i++)
	{
		pptc[i] = NULL; /* initialize pptc */
	}

	for (i = 0; shstruct(NULL)->newalias[i]; i++) /* get array of new alias names */
	{
		idx = char_srchANY(shstruct(NULL)->newalias[i], '=');
		if (idx) /* note that idx will recieve 0 above if '=' is first char */
		{
			ptc = strdup2(shstruct(NULL)->newalias[i]);
			ptc[idx] = 0; /* replace '=' with '\0' in duplicated string */
			pptc[j++] = ptc;
		}
	}

	for (i = 0; pptc[i]; i++)
	{
		if (_strcmp(cmd_name, pptc[i]) == 0) /* new alias name mentioned twice */
		{
			for (i = 0; pptc[i]; i++)
			{
				free(pptc[i]);
			}
			free(pptc);

			return (1);
		}
	}

	for (i = 0; pptc[i]; i++)
	{
		free(pptc[i]);
	}
	free(pptc);

	return (0);
}
