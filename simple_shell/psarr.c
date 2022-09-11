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
 * psarr - prints a NULL-terminated array of strings.
 * @sarr - the string array to print.
 * @c: the boundry char
 */
void psarr(char **sarr, char c)
{
	int i;

	if (!sarr)
	{
		return;
	}

	fprintf2(STDOUT_FILENO, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
			c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c);
	for (i = 0; sarr[i]; i++)
	{
		fprintf2(STDOUT_FILENO, "%s\n", sarr[i]);
	}

	fprintf2(STDOUT_FILENO, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
			c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c);
}
