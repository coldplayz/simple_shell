#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "main.h"

#define BUFF_SIZE 512


/**
 * getline2 - gets a line from stdin.
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
 * Return: the number of characters read from stdin.
 */
ssize_t getline2(char **line, size_t *n, FILE * stream __attribute__((unused)))
{
	char *buff;
	ssize_t m;
	int c;
	size_t i, bsize = BUFF_SIZE;

	m = 0;
	buff = malloc(sizeof(char) * bsize);
	if (buff == NULL)
	{
		printf("Malloc error\n");
		exit(EXIT_FAILURE);
	}

	i = 0;
	c = getchar();
	while (c != '\n' && c != EOF)
	{
		buff[i] = c;
		++m;
		++i;

		if ((i + 1) >= bsize)
		{
			bsize += BUFF_SIZE;
			buff = realloc(buff, (sizeof(char) * bsize));
			if (buff == NULL)
			{
				printf("Realloc error\n");
				exit(EXIT_FAILURE);
			}
		}
		c = getchar();
	}
	buff[i++] = '\n';
	buff[i] = 0;

	*n = bsize;
	*line = buff;
	return (m);
}
