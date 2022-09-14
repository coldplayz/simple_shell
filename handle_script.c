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
 * handle_script - gets the contents of a script file.
 * @name: name of file to read from.
 * @line: the address of a pointer to char buffer.
 * @n: the address of a long unsigned int representing the size of line.
 *
 * Description: handle_script() stores the line in a buffer (buff) with a
 * size equal to the smallest multiple of 512 bytes that can contain the
 * line string. line ill be null-terminated by fuction end.
 * line and n are modified to store
 * the address of the line string and buffer size respectively.
 * If argument 'line' is NULL, then n should be 0, otherwise supply
 * a [malloc'd] pointer as 'line' and the size as n.
 * Return: the number of characters read from the file, or 0 on EOF.
 */
ssize_t handle_script(char *name, char **line, size_t *n)
{
	int a, fdrd;
	ssize_t m;
	size_t i;
	unsigned int bsize = BUFSIZE, old_bsize = 0;
	char *buff;
	struct stat st;

	shstruct(NULL)->quick_exit = 0;
	if (stat(name, &st) != 0) /* file named 'name' does not exist */
	{
		fprintf2(STDERR_FILENO, "%s: 0: Can't open %s\n", shstruct(NULL)->name, name);
		exit(127);
	}

	fdrd = open(name, O_RDONLY);
	if (fdrd == -1)
	{
		perror("handle_script-open");
		exit(127);
	}

	m = 0;
	buff = malloc(sizeof(char) * bsize);
	if (buff == NULL)
	{
		perror("Malloc-buff");
		exit(127);
	}
	_memset(buff, 0, bsize);

	for (i = 0; 1; i++)
	{
		a = read(fdrd, (buff + old_bsize), BUFSIZE);
		if (a == -1)
		{
			free(buff);
			perror("stdin-read");
			exit(127);
		}
		m += a;

		if (a >= BUFSIZE)
			handle_realloc2(&buff, &old_bsize, &bsize, *line);
		else if ((a >= 0) && (a < BUFSIZE)) /* end of transmission/input */
		{
			old_bsize += a;
			close(fdrd);
			return (EOF_handler(&buff, old_bsize, a, m, n, line, bsize));
		}
	}

	*n = bsize;
	*line = buff;
	return (m);
}
