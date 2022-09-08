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
 * main - a simple shell
 * @argc: the number of main arguments.
 * @argv: array of argument strings.
 * @envp: the shell's environment.
 *
 * Return: always 0
 */
int main(int argc __attribute__((unused)), char *argv[], char *envp[])
{
	shell_t shell;
	size_t n = 0;
	int status = 0, a = 1, b = 1, _free = 0;
	char *line, *bltin_nm[]
		= {"exit", "setenv", "unsetenv", "cd", "env", "alias", NULL};

	init_shell(&shell, argv[0]);
	shstruct(&shell);
	while (a)
	{
		shstruct(NULL)->loop_cnt++;
		b = 1;
		line = NULL;
		if (isatty(STDIN_FILENO) == 1)
		{
			fprintf2(STDOUT_FILENO, "$ ");
			if (getline3(&line, &n, stdin) == 0)
			{
				status = 1;
				break;
			}
		}
		else
		{
			shell.quick_exit = 0;
			if (getline3(&line, &n, stdin) == 0)
			{
				status = 1;
				break;
			}
		}

		a = parser_launcher(line, &envp, bltin_nm, &b, &status, &_free, argv[0]);
		a = shell.quick_exit;
	}
	if (_free)
		handle_free("e", 0, envp);
	handle_free("e", 0, shstruct(NULL)->alias);

	return (status);
}

/* ===============NOTES=============== */
/* L60: b indicates whether to free str_ar[0] or not. 1==free, 0 otherwise. */
