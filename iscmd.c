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

int is_bltin(char *cmd_name);


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
 * Note: new and existing aliases will return 0 as long as
 * the "alias" command has been seen, and no non-command
 * string has been encountered to prompt a change in
 * the flag, indicating the presence of the alias command, from 1 to 0.
 * Return: 1 if cmd_name is in PATH or is a valid pathname; otherwise 0
 */
int iscmd(char *cmd_name, char **envp)
{
	char *path, **pathsv, *paths_cpy, *paths = getenv3("PATH", envp);
	struct stat st;
	int i;

	if (is_bltin(cmd_name))
		return (1);
	if (is_newalias(cmd_name))
	{
		if (shstruct(NULL)->is_aliascmd)
		{
			return (0);
		}
		else
		{
			return (1);
		}
	}
	if (isalias(cmd_name))
	{
		if (shstruct(NULL)->is_aliascmd)
		{
			return (0);
		}
		else
		{
			return (1);
		}
	}
	if (stat(cmd_name, &st) == 0)
	{
		if (isexec(&st, cmd_name))
			return (1);
	}

	if (!paths || paths[0] == 0)
	{
		return (0);
	}
	else
	{
		paths_cpy = strdup2(paths);
		pathsv = str_arr(paths_cpy, ":\0");
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


/**
 * is_bltin - checks whether a string represents a built-in command.
 * @cmd_name: the string to check.
 *
 * Return: 1 if built-in; 0 otherwise.
 */
int is_bltin(char *cmd_name)
{
	int i;

	for (i = 0; shstruct(NULL)->bltin_nm[i]; i++)
	{
		if (_strcmp(shstruct(NULL)->bltin_nm[i], cmd_name) == 0)
		{
			return (1);
		}
	}

	return (0);
}
