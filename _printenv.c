#include "main.h"
#include <stdio.h>
#include <stdlib.h>

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
		printf("%s\n", (*envp)[i]);
		i++;
	}

	return (1);

}
