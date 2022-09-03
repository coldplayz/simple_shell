#include "main.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * _printenv: prints the environment.
 *
 * envp: enviroment variable'
 * Return: 1 on success.
 */

int _printenv(char **envp)
{
	unsigned int i;

	i = 0;

	while (envp[i] != NULL)
	{
		printf("%s\n", envp[i]);
		i++;
	}

	return (1);

}
