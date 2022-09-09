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

#define ps(x) (printf("%s\n", (x)))
#define pd(x) (printf("%ld\n", (x)))
#define plu(x) (printf("%lu\n", (x)))


/**
 * isscript - an extension of is_binsh
 * that determines whether a file is a script, or not.
 * @buff: pointer to the storage of the first four characters of the file.
 * @sh: pointer to the string "#!".
 *
 * Return: 1 if file is script; 0 otherwise.
 */
int isscript(char *buff, char *sh)
{
	int i;

	for (i = 0; sh[i]; i++) /* test for script */
	{
		if (buff[i] != sh[i])
		{
			return (0);
		}
	}

	return (1);
}
