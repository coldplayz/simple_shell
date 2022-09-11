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
 * is_mult_cmd - checks if line has token(s) indicating multiple commands.
 * @line: the string to search in.
 *
 * Return: 1 if true; 0 otherwise.
 */
int is_mult_cmd(char *line)
{
	if (char_srch(line, ';'))
	{
		return (1);
	}

	if (char_srch(line, '&'))
	{
		return (1);
	}

	if (char_srch(line, '|'))
	{
		return (1);
	}

	return (0);
}
