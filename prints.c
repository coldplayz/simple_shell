#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "main.h"

/**
 * prints - prints the string s
 * @s: a pointer to the string to print
 *
 * Return: a pointer to an array of two integers, respectively
 * representing number of bytes written/printed to STDOUT,
 * and a flag indicating whether the string was empty,
 * where 1 means empty, and 0 otherwise.
 */
int *prints(char *s)
{
	int  bytes_written;

	if (s == NULL)
	{
		printf3.pti[0] = 6;
		printf3.pti[1] = 0;
		write(1, "(null)", 6);
		return (printf3.pti);
	}
	bytes_written = 0;
	bytes_written = write(printf3.fd, s, _strlen(s));
	if (bytes_written == -1)
	{
		perror("write-prints");
		return (printf3.pti);
	}

	if (bytes_written == 0)
	{
		printf3.pti[0] = 0;
		printf3.pti[1] = 1;
	}
	else
	{
		printf3.pti[0] = bytes_written;
		printf3.pti[1] = 0;
	}
	return (printf3.pti);
}
