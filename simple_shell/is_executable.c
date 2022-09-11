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



/**
 * isexec - checks whether a file is a regular file.
 * @st: a struct stat object containing info about the file.
 * @filename: name of file.
 *
 * Return: 1 if a regular file and binary executable, or script; 0 otherwise.
 */
int isexec(struct stat *st, char *filename)
{
	if (!(S_ISREG(st->st_mode)))
	{
		return (0);
	}

	return (is_binsh(filename));
}
