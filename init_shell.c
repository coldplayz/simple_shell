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

	shell->loop_cnt = 0;
	shell->name = av0;

	shell->quick_exit = 1;

	shell->null_term = 0;

	shell->quote = 0;
}
