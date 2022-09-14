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

ssize_t EOF_handler(char **buff, unsigned int old_size, int a,
		ssize_t m, size_t *n, char **line, unsigned int bsize);


/**
 * EOF_handler - a helper function for getline3().
 * @buff: address of char pointer where terminal input is stored.
 * @old_bsize: an unsigned int pointing to the next read position in buff.
 * @a: an int representing the number of last input characters read.
 * @m: the number of characters read thus far.
 * @n: address of int to store the size of buff in.
 * @line: address of char pointer to store reference to input string at.
 * @bsize: total size of buff.
 *
 * Return: 0 always; to be returned by the shell's launcher.
 */
ssize_t EOF_handler(char **buff, unsigned int old_bsize,
		int a, ssize_t m, size_t *n, char **line, unsigned int bsize)
{
	char *ptc;

	(void)a;

	if (!val_line(buff)) /* input end: check all xters written to buff so far*/
	{
		free(shstruct(NULL)->alias);
		free(*buff);
		fprintf2(STDERR_FILENO, "\n");
		return (0);
	}

	*n = bsize;
	(*buff)[old_bsize] = 0;
	*line = *buff;

	/* check for empty lines */
	ptc = strdup2(*line);
	strtok2(ptc, " \n\t\0", 0);
	if (_strlen(ptc) == 0)
	{
		free(shstruct(NULL)->alias);
		free(ptc);
		free(*line);
		return (0);
	}

	free(ptc);

	return (m);
}
