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

int mult_cmd_launcherSEM(char *shell_nm, int *b, char ***envp,
		int *status, int *_free, char **bltin_nm, char *line);

int mult_cmd_launcherAND(char *shell_nm, int *b, char ***envp,
		int *status, int *_free, char **bltin_nm, char *line);

int mult_cmd_launcherOR(char *shell_nm, int *b, char ***envp,
		int *status, int *_free, char **bltin_nm, char *line);

int mclTTY(char *shell_nm, int *b, char ***envp,
		int *status, int *_free, char **bltin_nm, char *line);

/**
 * parser_launcher - handles the shell's command parsing and launching.
 * @line: input string.
 * @envp: address of the process' environment.
 * @bltin_nm: array of names of built-in commands.
 * @b: address of int determining how to free the arrays of command strings.
 * @status: address of int storing exit status.
 * @_free: address of int determining whether or not to free envp.
 * @shell_nm: the shell's pathname.
 *
 * Return: always 1, except in certain cases like when exit() is called, then 0
 */
int parser_launcher(char *line, char ***envp,
		char **bltin_nm, int *b, int *status, int *_free, char *shell_nm)
{
	int c = 1, flag = 0;
	char *ptc, *ptc2, *ptc3, *ptc4, **str_ar;

	(void)flag;
	if (!(isatty(STDIN_FILENO)))
		pipe_parser(line, *envp);
	if (is_ORAND(line))
		return (mult_cmd_launcherORAND(shell_nm, b,
					envp, status, _free, bltin_nm, line));
	if (char_srch(line, ';'))
	{
		return (mult_cmd_launcherSEM(shell_nm, b,
					envp, status, _free, bltin_nm, line));
	}
	else if (char_srch(line, '&'))
	{
		return (mult_cmd_launcherAND(shell_nm, b,
					envp, status, _free, bltin_nm, line));
	}
	else if (char_srch(line, '|'))
	{
		return (mult_cmd_launcherOR(shell_nm, b,
					envp, status, _free, bltin_nm, line));
	}
	else
	{
		ptc4 = strdup2(line); /* free */
		strtok2(ptc4, " \n\0", 0);
		if (isalias(ptc4))
		{
			ptc = getalias(strtok2(line, " \n\0", 0)); /* ptc is pointing right into the alias array */
			ptc2 = strdup2(ptc); /* create a copy so as not to modify the alias array */
			ptc3 = strtok2(ptc2, "\'", 0); /* strip away the '' delimiters */
			free(line);
			line = ptc3;
			flag = 1; /* indicates to free ptc2 */
		}
		free(ptc4);
		str_ar = in_parser(line, *envp, bltin_nm, b);
		if (!str_ar) /* command not found */
		{
			free(line);
			shell_nm = shell_nm;
			return (1);
		}
		if (str_ar[0] == NULL) /* no input */
		{
			handle_free("sd", shstruct(NULL)->free0, line, str_ar);
			return (1);
		}
		c = launcher(str_ar, envp, bltin_nm, status, _free);
		handle_free("d", shstruct(NULL)->free0, str_ar);
		*b = 1;
		if (flag)
		{
			free(ptc2);
		}
		else
		{
			free(line);
		}
	}

	return (c);
}

/* ===============NOTES=============== */
/* L60: b indicates whether to free str_ar[0] or not. 1==free, 0 otherwise. */


/**
 * mult_cmd_launcherSEM - handles the shell's command parsing and launching.
 * @line: input string.
 * @envp: address of the process' environment.
 * @bltin_nm: array of names of built-in commands.
 * @b: address of int determining how to free the arrays of command strings.
 * @status: address of int storing exit status.
 * @_free: address of int determining whether or not to free envp.
 * @shell_nm: the shell's pathname.
 *
 * Return: always 1, except in certain cases like when exit() is called, then 0
 */
int mult_cmd_launcherSEM(char *shell_nm, int *b, char ***envp,
		int *status, int *_free, char **bltin_nm, char *line)
{
	int a, c = 1, flag = 0;
	char **str_ar, **str_ar2, *ptc, *ptc2, *ptc3;

	str_ar = str_arr(line, ";\n\0"); /* get array of strings of multiple commands */

	for (a = 0; (a < str_arrlen(str_ar) && c); a++) /* run subshell 4 each cmd */
	{
		if (isalias(str_ar[a]))
		{
			ptc = getalias(str_ar[a]); /* ptc is pointing right into the alias array */
			ptc2 = strdup2(ptc); /* create a copy so as not to modify the alias array */
			ptc3 = strtok2(ptc2, "\'", 0); /* strip away the '' delimiters */
			str_ar[a] = ptc3;
			flag = 1; /* indicates to free ptc2 */
		}

		str_ar2 = in_parser(str_ar[a], *envp, bltin_nm, b);
		if (!str_ar2) /* command not found */
		{
			shell_nm = shell_nm;
			c = 1;
			continue;
		}
		if (str_ar2[0] == NULL) /* no input - only newline and null character */
		{
			handle_free("d", shstruct(NULL)->free0, str_ar2);
			c = 1;
			continue;
		}

		c = launcher(str_ar2, envp, bltin_nm, status, _free);

		handle_free("d", shstruct(NULL)->free0, str_ar2);
		*b = 1;
		if (flag)
		{
			free(ptc2);
		}
	}
	handle_free("sd", 0, line, str_ar);

	return (c);
}



/**
 * mult_cmd_launcherAND - handles the shell's command parsing and launching.
 * @line: input string.
 * @envp: address of the process' environment.
 * @bltin_nm: array of names of built-in commands.
 * @b: address of int determining how to free the arrays of command strings.
 * @status: address of int storing exit status.
 * @_free: address of int determining whether or not to free envp.
 * @shell_nm: the shell's pathname.
 *
 * Return: always 1, except in certain cases like when exit() is called, then 0
 */
int mult_cmd_launcherAND(char *shell_nm, int *b, char ***envp,
		int *status, int *_free, char **bltin_nm, char *line)
{
	int a, c = 1;
	char **str_ar, **str_ar2;

	str_ar = str_arr(line, "&\n\0"); /* get array of strings of multiple commands */

	for (a = 0; (a < str_arrlen(str_ar) && c); a++) /* run subshell 4 each cmd */
	{
		str_ar2 = in_parser(str_ar[a], *envp, bltin_nm, b);
		if (!str_ar2) /* command not found */
		{
			shell_nm = shell_nm;
			c = 1;
			break;
		}
		if (str_ar2[0] == NULL) /* no input - only newline and null character */
		{
			handle_free("d", shstruct(NULL)->free0, str_ar2);
			c = 1;
			continue;
		}

		c = launcher(str_ar2, envp, bltin_nm, status, _free);

		handle_free("d", shstruct(NULL)->free0, str_ar2);
		*b = 1;
		if (shstruct(NULL)->exstat != 0)
		{
			break;
		}
	}
	handle_free("sd", 0, line, str_ar);

	return (c);
}





/**
 * mult_cmd_launcherOR - handles the shell's command parsing and launching.
 * @line: input string.
 * @envp: address of the process' environment.
 * @bltin_nm: array of names of built-in commands.
 * @b: address of int determining how to free the arrays of command strings.
 * @status: address of int storing exit status.
 * @_free: address of int determining whether or not to free envp.
 * @shell_nm: the shell's pathname.
 *
 * Return: always 1, except in certain cases like when exit() is called, then 0
 */
int mult_cmd_launcherOR(char *shell_nm, int *b, char ***envp,
		int *status, int *_free, char **bltin_nm, char *line)
{
	int a, c = 1;
	char **str_ar, **str_ar2;

	str_ar = str_arr(line, "|\n\0"); /* get array of strings of multiple commands */

	for (a = 0; (a < str_arrlen(str_ar) && c); a++) /* run subshell 4 each cmd */
	{
		str_ar2 = in_parser(str_ar[a], *envp, bltin_nm, b);
		if (!str_ar2) /* command not found */
		{
			shell_nm = shell_nm;
			c = 1;
			continue;
		}
		if (str_ar2[0] == NULL) /* no input - only newline and null character */
		{
			handle_free("d", shstruct(NULL)->free0, str_ar2);
			c = 1;
			continue;
		}

		c = launcher(str_ar2, envp, bltin_nm, status, _free);

		handle_free("d", shstruct(NULL)->free0, str_ar2);
		*b = 1;

		if (shstruct(NULL)->exstat == 0)
		{
			break;
		}
	}
	handle_free("sd", 0, line, str_ar);

	return (c);
}




/**
 * mclTTY - handles the shell's command parsing and launching.
 * @line: input string.
 * @envp: address of the process' environment.
 * @bltin_nm: array of names of built-in commands.
 * @b: address of int determining how to free the arrays of command strings.
 * @status: address of int storing exit status.
 * @_free: address of int determining whether or not to free envp.
 * @shell_nm: the shell's pathname.
 *
 * Return: always 1, except in certain cases like when exit() is called, then 0
 */
int mclTTY(char *shell_nm, int *b, char ***envp,
		int *status, int *_free, char **bltin_nm, char *line)
{
	int a, c = 1;
	char *line_cpy, **str_ar, **str_ar2;

	line_cpy = strdup2(line);
	str_ar = str_arr(line_cpy, " \n\0"); /* get array of strings of multiple commands */
	if (!(iscmd(str_ar[1], *envp)))
	{
		free(line_cpy);
		return (mclTTY2(shell_nm, b, envp, status, _free, bltin_nm, line));
	}

	for (a = 0; (a < str_arrlen(str_ar) && c); a++) /* run subshell 4 each cmd */
	{
		str_ar2 = in_parser(str_ar[a], *envp, bltin_nm, b);
		if (!str_ar2) /* command not found */
		{
			shell_nm = shell_nm;
			c = 1;
			continue;
		}
		if (str_ar2[0] == NULL) /* no input - only newline and null character */
		{
			handle_free("d", shstruct(NULL)->free0, str_ar2);
			c = 1;
			continue;
		}

		c = launcher(str_ar2, envp, bltin_nm, status, _free);

		handle_free("d", shstruct(NULL)->free0, str_ar2);
		*b = 1;
	}
	handle_free("ssd", 0, line, line_cpy, str_ar);

	return (c);
}
