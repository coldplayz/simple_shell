#include <sys/types.h>
#include <sys/stat.h>
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
#define plu(x) (printf("%lu", (x)))


/**
 * val_line - ensures that my function, getline3(), delivers a line of input.
 * @input: address of a buffer storing data fetched from stdin.
 *
 * Return: 1 on finding a newline character; 0 otherwise.
 */
int val_line(char **input)
{
	int i;

	for (i = 0; (*input)[i]; i++)
	{
		if ((*input)[i] == '\n')
		{
			/* (*input)[++i] = 0; */
			return (1);
		}
	}

	return (0);
}
