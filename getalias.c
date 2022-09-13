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
 * getalias - gets the base/real value of an alias.
 * @cmd_name: program name
 *
 * Return: pointer to the alias value at the lowest level, or NULL.
 */
char *getalias(char *cmd_name)
{
	int i;
	char *ptc, *ptc2, *ptc3, *bkptc;

	ptc = isalias(cmd_name);
	if (!ptc)
	{
		return (NULL);
	}

	for (i = 0; ptc; i++)
	{
		ptc2 = ptc;
		ptc3 = strdup2(ptc);
		bkptc = strtok2(ptc3, "\'\0", 0);
		ptc = isalias(bkptc);
		free(ptc3);
	}

	return (ptc2); /* expect ptc2 to be delimited with '' */
}
