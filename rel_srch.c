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

#define ex (execve("/bin/ls", args, NULL) == (-1))
#define ps(x) (printf("%s\n", (x)))
#define pd(x) (printf("%d\n", (x)))

/**
 * rel_srch - checks if cmd is a valid path to a file.
 * @cmd: the string representing the pathname
 *
 * Return: 1 if the pathname is valid; 0 otherwise.
 */
int rel_srch(char *cmd)
{
	struct stat st;

	if (_strcmp(cmd, "$?") == 0)
	{
		/* sarr[0] = itoa2(shstruct(NULL)->exstat); itoa2 returns a malloc'd ptc */
		/* shstruct(NULL)->freeitoa = 1; */
		return (1);
	}
	else if (_strcmp(cmd, "$$") == 0)
	{
		/* sarr[0] = itoa2(pid); */
		/* shstruct(NULL)->freeitoa = 1; */
		return (1);
	}
	else if (cmd[0] == '$')
	{
		/* sarr[0] = getenv3(sarr[0] + 1, *shstruct(NULL)->envp); */
		return (1);
	}

	if (stat(cmd, &st) != 0)
	{
		return (0);
	}

	return (1);
}
