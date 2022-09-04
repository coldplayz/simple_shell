#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "main.h"


#define ps(x) printf("%s\n", x)
#define pd(x) printf("%d\n", x)
#define plu(x) printf("%lu\n", x)

/**
 * del_env - deletes the entry specified by name in the environment list.
 * @name: variable name.
 * @edit_idx: the index of the variable to delete.
 * @envp: the process' environment.
 * @free: address of int indicating whether to free envp.
 *
 * Description: this function is an extension of setenv2(),
 * and moves environ to a dynamically allocated memory space.
 * Helper:
 * 1. handle_dpmall()
 * 2. strdup2()
 * Return: 1 always for the shell.
 */
int del_env(const char *name, int edit_idx, char ***envp, int *free)
{
	int i, j, env_len;
	char **buff;

	(void)name;
	env_len = str_arrlen(*envp);

	buff = malloc(sizeof(char *) * (env_len));
	if (handle_dpmall(buff))
	{
		return (1);
	}

	j = 0;
	for (i = 0; j < (env_len); i++)
	{
		/**
		 * The IF block ensures that the variable entry to be deleted
		 * is not copied to buff. The ELSE block ensures that when the
		 * index of the variable to be removed from the environ is
		 * skipped, the corresponding index in the buff isn't.
		 */
		if (edit_idx != j)
			buff[i] = strdup2((*envp)[j]);
		else
			--i;
		j++;
	}
	buff[i] = NULL;

	if (*free)
		handle_free("e", 0, *envp);
	*free = 1;
	*envp = buff;

	return (1);
}
