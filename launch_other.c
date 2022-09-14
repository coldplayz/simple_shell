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
 * launch_other - launches programs that are not builtins.
 * @sarr: array of strings containing command-line arguments for the program.
 * @envp: array of strings containing the process's environment.
 *
 * Notes: the program pathname represented by sarr[0] must exist.
 * Return: always 1.
 */
int launch_other(char **sarr, char **envp)
{
	int i, status, flag = -1;
	pid_t pid, pid1;

	pid = getpid();

	for (i = 0; sarr[i]; i++)
	{
		if (_strcmp(sarr[i], "$?") == 0)
		{
			sarr[i] = itoa2(shstruct(NULL)->exstat); /* itoa2 returns a malloc'd ptc */
			flag = i;
		}
		else if (_strcmp(sarr[i], "$$") == 0)
		{
			sarr[i] = itoa2(pid);
			flag = i;
		}
		else if ((sarr[i][0] == '$') && (sarr[i][1] != ' ' && sarr[i][1] != '\n' && sarr[i][1] != '\t' && sarr[i][1] != '\0'))
		{
			sarr[i] = getenv3(sarr[i] + 1, *shstruct(NULL)->envp);
		}
	}
	if (sarr[0] == NULL)
	{
		for (i = 1; sarr[i]; i++) /* free memory allocated by call to itoa2 */
		{
			/* starts from sarr[1] otherwise as sarr[0] */
			/* is NULL, entrance wouldn't be allowed into this loop */
			if (flag == i)
			{
				free(sarr[i]);
			}
		}
		fprintf2(STDOUT_FILENO, "\n");
		return (1);
	}

	pid1 = fork();
	if (pid1 == -1)
	{
		for (i = 0; sarr[i]; i++)
		{
			if (flag == i)
			{
				free(sarr[i]);
			}
		}
		perror("launch_other-fork");
		return (1);
	}
	else
	{
		if (pid1 == 0)
		{
			if (execve(sarr[0], sarr, envp) == -1)
			{
				for (i = 0; sarr[i]; i++)
				{
					if (flag == i)
					{
						free(sarr[i]);
					}
				}
				perror("launch_other-execve");
				return (1);
			}
		}
		else
		{
			wait(&status);
			if (WIFEXITED(status))
			{
				shstruct(NULL)->exstat = WEXITSTATUS(status);
			}

			for (i = 0; sarr[i]; i++)
			{
				if (flag == i)
				{
					free(sarr[i]);
				}
			}
			return (1);
		}
	}

	return (1);
}
