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
	int c = 1;
	char **str_ar;

	if (!(isatty(STDIN_FILENO)))
		pipe_parser(line, *envp);
	if (is_ORAND(line))
		return (mult_cmd_launcherORAND());
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
	}
}





int mult_cmd_launcherORAND(char *line)
{
	int i, map[10];

	ORAND_map(line, map);
	for (i = 0; i < arrlen; i++)
	{
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
		handle_free("sd", shstruct(NULL)->free0, line, str_ar);
		*b = 1;
	}

	return (c);
}



int ORAND_map(char *line, int *map)
{
	int i, val;

	val = is_ORAND3(line);
	if (!val)
	{
		return (0);
	}
	
	for (i = 0; val; i++)
	{
		map[i] = val;
		val = is_ORAND2(NULL);
	}

	return (i++); /* number of commands returned */
}





/**
 * strtok2 - tokenize a string, str, based on delimiters, delim.
 * @str: the string to tokenize.
 * @delim: delimiter characters.
 *
 * Description: works similar to the library's version. See strtok(3).
 * Note: modifies the characters in str. So, not safe.
 * Return: the address of the current extracted token, or NULL.
 */
int is_ORAND3(char *str)
{
	static char *ptc;
	int i;

	if (str != NULL)
		ptc = str;
	if (*ptc == '\0')
		return (0);
	for (i = 0; 1; i++)
	{
		val = is_ORAND2(str);
		if (val)
		{
			ptc = ptc + 2; /* Point to next character for the next call */
			return (val);
		}
		else
		{
			return (0);
		}

		if ((*ptc == '\0')) /*none was seen for this call by string end*/
			return (0);
		ptc++;
	}
	return (0);
}




/**
 * is_ORAND2 - searches for the presence of "&&" and/or "||" tokens in str.
 * @str: string to search in.
 *
 * Return: 1 if "||" is found or
 * 2 if "&&" is found, whichever is first found; and 0 otherwise.
 */
int is_ORAND2(char *str)
{
	int i = 0, flag1 = 0, flag2 = 0;
	char *ptc1, ptc2;

	for (i = 0; str[i]; i++)
	{
		if (str[i] == '|')
		{
			if (str[++i] == '|')
			{
				flag1 = 3;
				ptc1 = str + i;
			}
			else
			{
				break;
			}
		}
	}

	for (i = 0; str[i]; i++)
	{
		if (str[i] == '&')
		{
			if (str[++i] == '&')
			{
				flag2 = 3;
				ptc2 = str + i;
			}
			else
			{
				break;
			}
		}
	}

	if (!flag1)
	{
		if (flag2)
		{
			return (2); /* && was found but || wasn't */
		}
		else
		{
			return (0); /* neither was found */
		}
	}
	else if (flag1)
	{
		if (!flag2)
		{
			return (1); /* || was found but && wasn't */
		}
		else
		{
			if (ptc1 < ptc2) /* both were found */
			{
				return (1);
			}
			else
			{
				return (2);
			}
		}
	}

	return (0);
}
