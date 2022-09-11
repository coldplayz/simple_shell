#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "main.h"
#define ex (execve("/bin/ls", args, NULL) == (-1))
#define ps(x) (printf("%s\n", (x)))
#define pd(x) (printf("%d\n", (x)))
#define plu(x) (printf("%lu\n", (x)))



/**
 * handle_strlen2 - handles calcuation of string length 4 setenv2 and edit_env.
 * @str: the string whose length to calcuate.
 *
 * Return: length of str; or 0 if NULL.
 */
int handle_strlen2(char *str)
{
	if (!str)
	{
		return (0);
	}
	
	return (_strlen(str));
}
