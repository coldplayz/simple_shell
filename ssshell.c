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
	int i, status = 0, a = 1, b = 1, _free = 0, noscript = 1;
	char *line = NULL, **bltin_nm = shell.bltin_nm;

	init_shell(&shell, argv[0]); /* initialize the shell's struct */
	shell.envp = &envp;
	shstruct(&shell); /* store the address of the shell's struct in shstruct */
	if (argv[1])
	{
		handle_script(argv[1], &line, &n);
		noscript = 0;

		for (i = 0; line[i]; i++)
		{
			if (line[i] == '\n')
			{
				/* replace '\n' with command separator */
				/* to allow entrance into mult_cmd_launcherSEM() */
				line[i] = ';';
			}
		}
	}
	while (a)
	{
		shstruct(NULL)->loop_cnt++;
		b = 1;
		if (noscript)
		{
			line = NULL;
		}
		if (isatty(STDIN_FILENO) == 1 && noscript)
		{
			fprintf2(STDOUT_FILENO, "$ ");
			if (getline3(&line, &n, stdin) == 0)
			{
				status = 1;
				break;
			}
			strtok2(line, "#", 0); /* remove comments */
		}
		else if (noscript)
		{
			shell.quick_exit = 0;
			if (getline3(&line, &n, stdin) == 0)
			{
				status = 1;
				break;
			}
			strtok2(line, "#", 0); /* remove comments */
		}

		a = parser_launcher(line, &envp, bltin_nm, &b, &status, &_free, argv[0]);
		a = shell.quick_exit;
	}
	if (_free)
		handle_free("e", 0, envp);
	handle_free("e", 0, shstruct(NULL)->alias);

	return (shstruct(NULL)->exstat);
}

/* ===============NOTES=============== */
/* L60: b indicates whether to free str_ar[0] or not. 1==free, 0 otherwise. */
