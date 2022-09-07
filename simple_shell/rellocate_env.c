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
 * rellocate_env - creates a new buffer space for
 * the process' environment stored in main's third argument..
 * @name: variable name.
 * @val: variable value.
 * @edit_idx: the index for the variable
 * to update, if cflag indicates overwrite.
 * @cflag: an int flag indicating whether to overwrite (1),
 * or create a new entry (2) in the environment list.
 * @envp: the environment.
 * @free: int indicating whether to free envp (1), or not (0).
 *
 * Description: this function is an extension of setenv2(),
 * and moves environ to a dynamically allocated memory space.
 * Helper:
 * 1. handle_dpmall()
 * 2. strconcatl()
 * 3. strdup2()
 * Return: always 1 for the shell's loop to continue.
 */
int rellocate_env(const char *name, const char *val,
		int edit_idx, int cflag, char ***envp, int *free)
{
	int i, env_len;
	char **buff;

	env_len = str_arrlen(*envp);
	switch (cflag)
	{
		case 1:
			/* for overwrite */
			buff = malloc(sizeof(char *) * (env_len + 1));
			if (handle_dpmall(buff))
				return (1);
			for (i = 0; i < (env_len); i++)
			{
				if (i == edit_idx)
					buff[i] = strconcatl(3, name, "=", val);
				else
					buff[i] = strdup2((*envp)[i]);
			}
			buff[i] = NULL;
			if (*free)
				handle_free("e", 0, *envp);
			*free = 1; /* subsequent rellocations will perform L1 free on envp */
			break;
		case 2:
			/* for new entry */
			buff = malloc(sizeof(char *) * (env_len + 2));
			if (handle_dpmall(buff))
				return (1);
			for (i = 0; i < env_len; i++)
				buff[i] = strdup2((*envp)[i]);
			buff[i++] = strconcatl(3, name, "=", val);
			buff[i] = NULL;
			if (*free)
				handle_free("e", 0, *envp);
			*free = 1;
	}
	*envp = buff;

	return (1);
}
