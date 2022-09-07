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
		shstruct(NULL)->pti[0] = 6;
		shstruct(NULL)->pti[1] = 0;
		write(1, "(null)", 6);
		return (shstruct(NULL)->pti);
	}
	bytes_written = 0;
	bytes_written = write(shstruct(NULL)->fd, s, _strlen(s));
	if (bytes_written == -1)
	{
		perror("write-prints");
		return (shstruct(NULL)->pti);
	}

	if (bytes_written == 0)
	{
		shstruct(NULL)->pti[0] = 0;
		shstruct(NULL)->pti[1] = 1;
	}
	else
	{
		shstruct(NULL)->pti[0] = bytes_written;
		shstruct(NULL)->pti[1] = 0;
	}
	return (shstruct(NULL)->pti);
}
