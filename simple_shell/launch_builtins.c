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


/**
 * launch_builtins - launches builtin shell programs.
 * @sarr: pointer to the string array
 * containing program name, and, possibly, arguments.
 * @envp: address of the environment.
 * @n: the index pointing to the function to invoke.
 * @status: the address of an int to be modified by the exit builtin.
 * @free: address of an int indicating whether to free envp (1), or not (0).
 *
 * Description: all programs (builtins) launched by this function
 * must take a double pointer to
 * command-line input, a double pointer to the environment,
 * a pointer to int for exit status, and a pointer to
 * int for freeing the environment (or not), and return 1, or 0.
 * Notes: the program pathname represented by sarr[0] must exist.
 * Return: always 1 (the return value of the program it launches);
 * except when the shell program returns 0, as in the case of exit().
 */
int launch_builtins(char **sarr, char ***envp, int n, int *status, int *free)
{
	int (*bltin_func[])(char **, char ***, int *, int *)
		= {exit2, setenv2, unsetenv2, cd2, _printenv, alias2, NULL};

	return (bltin_func[n](sarr, envp, status, free));
}
