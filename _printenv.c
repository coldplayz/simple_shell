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
 * _printenv - prints the environment.
 * @sarr: array of strings containing command-line input.
 * @envp: address of environment.
 * @status: address of status int.
 * @free: address of int indicating whether, or not, to free envp.
 *
 * Return: always 0.
 */

int _printenv(char **sarr, char ***envp, int *status, int *free)
{
	unsigned int i;

	(void)sarr;
	(void)status;
	(void)free;

	i = 0;

	while ((*envp)[i] != NULL)
	{
		fprintf2(STDOUT_FILENO, "%s\n", (*envp)[i]);
		i++;
	}

	return (1);

}
