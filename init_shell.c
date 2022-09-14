#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <dirent.h>
#include "main.h"

#define ex (execve("/bin/ls", args, NULL) == (-1))
#define ps(x) (printf("%s\n", (x)))
#define pd(x) (printf("%d\n", (x)))
#define plu(x) (printf("%lu", (x)))


void init_bltin_nm(shell_t *shell);

/**
 * init_shell - initializes the fields in the struct shell_t.
 * @shell: address of the struct to initialize.
 * @av0: the program's name.
 */
void init_shell(shell_t *shell, char *av0)
{
	shell->pti[0] = 0;
	shell->pti[1] = 0;

	shell->alias = malloc(sizeof(char *));
	handle_dpmall(shell->alias);
	shell->alias[0] = NULL;

	shell->is_aliascmd = 0;

	shell->loop_cnt = 0;
	shell->name = av0;

	shell->quick_exit = 1;

	shell->null_term = 0;

	shell->quote = 0;

	shell->free0 = 1;

	shell->exstat = 0;

	shell->noscript = 1;

	init_bltin_nm(shell);

}


/**
 * init_bltin_nm - initializes the array of names of built-in functions.
 * @shell: pointer to the shell struct.
 */
void init_bltin_nm(shell_t *shell)
{
	shell->bltin_nm[0] = "exit";

	shell->bltin_nm[1] = "setenv";

	shell->bltin_nm[2] = "unsetenv";

	shell->bltin_nm[3] = "cd";

	shell->bltin_nm[4] = "env";

	shell->bltin_nm[5] = "alias";

	shell->bltin_nm[6] = NULL;
}
