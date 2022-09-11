#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "main.h"

#define ps(x) printf("%s\n", x)
#define pd(x) printf("%d\n", x)

/**
 * str_arr - collects token strings into an array of strings.
 * @str: string to tokenize.
 * @delim: string made up of delimiter characters.
 *
 * Description: the pointers in the returned array
 * are simply addresses in str, not some malloc'd addresses.
 * Hence, you should only conduct L0 free.
 * Return: a NULL-teminated array of token strings.
 */
char **str_arr(char *str, const char *delim)
{
	char **str_ar, *token;
	int i, bsize = sizeof(char *), bsize_total = bsize;

	str_ar = malloc(bsize);
	if (!str_ar)
	{
		perror("str-arr-malloc");
		return (NULL);
	}

	token = strtok2(str, delim, shstruct(NULL)->quote);
	if (!token)
	{
		str_ar[0] = NULL;
		return (str_ar);
	}
	i = 0;
	while (token)
	{
		str_ar[i] = token;
		i++;
		token = strtok2(NULL, delim, shstruct(NULL)->quote);

		handle_realloc(&(str_ar), i, bsize, &bsize_total);

		if (!token)
		{
			str_ar[i] = NULL;
			break;
		}
	}

	return (str_ar);
}
