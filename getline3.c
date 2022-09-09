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
 * getline3 - gets a line from stdin.
 * @line: the address of a pointer to char buffer.
 * @n: the address of a long unsigned int representing the size of line.
 * @stream: the data stream to
 * read from represented as a pointer to a FILE object.
 *
 * Description: getline2() stores the line in a buffer (buff) with a
 * size equal to the smallest multiple of 512 bytes that can contain the
 * line string. Includes the newline character, followed by null-termination.
 * line and n are modified to store
 * the address of the line string and buffer size respectively.
 * If argument 'line' is NULL, then n should be 0, otherwise supply
 * a [malloc'd] pointer as 'line' and the size as n.
 * Return: the number of characters read from stdin, or 0 on EOF.
 */
ssize_t getline3(char **line, size_t *n, FILE * stream __attribute__((unused)))
{
	char *buff;
	ssize_t m;
	int a;
	size_t i;
	unsigned int bsize = BUFSIZE, old_bsize = 0;

	m = 0;
	buff = malloc(sizeof(char) * bsize);
	if (buff == NULL)
	{
		perror("Malloc-buff");
		exit(EXIT_FAILURE);
	}
	_memset(buff, 0, bsize);

	for (i = 0; 1; i++)
	{
		a = read(STDIN_FILENO, (buff + old_bsize), BUFSIZE);
		if (a == -1)
		{
			free(buff);
			perror("stdin-read");
			exit(EXIT_FAILURE);
		}
		m += a;

		if (a >= BUFSIZE)
			handle_realloc2(&buff, &old_bsize, &bsize, *line);
		else if ((a >= 0) && (a < BUFSIZE)) /* end of transmission/input */
		{
			old_bsize += a;
			return (EOF_handler(&buff, old_bsize, a, m, n, line, bsize));
		}
	}

	*n = bsize;
	*line = buff;
	return (m);
}


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
	(void)old_bsize;
	(void)a;

	if (!val_line(buff)) /* input end: check all xters written to buff so far*/
	{
		free(*buff);
		fprintf2(STDERR_FILENO, "\n");
		return (0);
	}

	*n = bsize;
	*line = *buff;

	return (m);
}
