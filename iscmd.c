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
 * iscmd - checks if a command/program name is
 * in any of the paths specified in the environment variable PATH.
 * This version combines code from isinPATH2 and rel_srch.
 * @cmd_name: program name
 * @envp: the process's environment.
 *
 * Helper:
 * 1. strconcatl()
 * 2. str_arr()
 * 3. getenv3()
 * Return: 1 if cmd_name is in PATH or is a valid pathname; otherwise 0
 */
int iscmd(char *cmd_name, char **envp)
{
	char *path, **pathsv, *paths_cpy, *paths = getenv3("PATH", envp);
	struct stat st;
	int i;

	if (stat(cmd_name, &st) == 0)
	{
		if (isexec(&st, cmd_name))
			return (1);
	}

	if (!paths)
	{
		write(STDERR_FILENO, "isinPATH2: pathnoval\n", 21);
		return (0);
	}
	else
	{
		paths_cpy = strdup2(paths);
		pathsv = str_arr(paths_cpy, ":");
	}
	for (i = 0; pathsv[i]; i++)
	{
		path = strconcatl(3, pathsv[i], "/", cmd_name);
		if (stat(path, &st) == 0)
		{
			if (isexec(&st, cmd_name))
			{
				free(paths_cpy);
				free(pathsv);
				free(path);
				return (1);
			}
		}
		free(path);
	}
	free(paths_cpy);
	free(pathsv);

	return (0);
}
