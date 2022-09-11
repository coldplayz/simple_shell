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

int ORAND_map(char *line, int *map);
int is_ORAND3(char *str);
int is_ORAND2(char **str);



/**
 * mult_cmd_launcherORAND - handles advaned parsing and
 *  launching of commands involving the "||" and "&&" logical operations.
 *  @line: string to parse for launching.
 *  @envp: environment.
 *  @bltin_nm: built-in command names.
 *  @b: an int pointer.
 *  @status: an int pointer.
 *  @_free: int pointer.
 *  @shell_nm: name of shell program.
 *
 *  Return: always 1, except exit was called, then 0.
 */
int mult_cmd_launcherORAND(char *shell_nm, int *b, char ***envp,
		int *status, int *_free, char **bltin_nm, char *line)
{
	int c, i, n, map[10];
	char **sarr, **str_ar2;

	n = ORAND_map(line, map);
	map[n - 1] = 0;
	sarr = str_arr(line, "&|\n\0");
	for (i = 0; i < n; i++)
	{
		str_ar2 = in_parser(sarr[i], *envp, bltin_nm, b);
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
		if ((shstruct(NULL)->exstat != 0) && (map[i] == 2))
		{
			break;
		}
		if ((shstruct(NULL)->exstat != 0) && (map[i] == 1))
		{
			continue;
		}
		if ((shstruct(NULL)->exstat == 0) && (map[i] == 1))
		{
			break;
		}
		if ((shstruct(NULL)->exstat == 0) && (map[i] == 2))
		{
			continue;
		}

	}
	handle_free("sd", 0, line, sarr);

	return (c);
}





/**
 * ORAND_map - populates an array with integers
 * indicating the type of shell logical operation to perform.
 * @line: the string to parse.
 * @map: the array of integers to populate.
 *
 * Return: the number of token-separated commands in line; or 0 if none.
 */
int ORAND_map(char *line, int *map)
{
	int i, val;

	val = is_ORAND3(line);
	if (!val)
	{
		return (0);
	}
	
	for (i = 0; val; i++) /* runs until 0 is returned by is_ORAND3 */
	{
		map[i] = val;
		val = is_ORAND3(NULL);
	}

	return (++i); /* return number of commands */
}





/**
 * is_ORAND3 - returns a value indicating the type of the next command token.
 * @str: the string to search for command tokens in.
 *
 * Description: the first call to this function should see
 * str supplied to it. It should return an appropriate int.
 * NULL should be passed on subsequent calls so long as the
 * caller needs the first string to be parsed.
 * Once str is done being parsed, the function returns 0.
 * Note: Does not modify the characters in str.
 * Return: 1 if "||" is seen; 2 if "&&" is seen; or 0 if none is seen..
 */
int is_ORAND3(char *str)
{
	static char *ptc;
	int i, val;

	if (str != NULL)
		ptc = str;
	if (*ptc == '\0')
		return (0);
	for (i = 0; 1; i++)
	{
		val = is_ORAND2(&ptc); /* ptc gets updated to point to next char */
		if (val)
		{
			return (val);
		}
		else
		{
			/* no "||" and/or "&&" in string represented by ptc */
			return (0);
		}
	}
	return (0);
}




/**
 * is_ORAND2 - searches for the presence of "&&" and/or "||" tokens in str.
 * @str: address of string to search in.
 *
 * Return: 1 if "||" is found or
 * 2 if "&&" is found, whichever is first found; and 0 otherwise.
 */
int is_ORAND2(char **str)
{
	int i1 = -1, i2 = -1, i = 0, flag1 = 0, flag2 = 0;
	char *ptc1, *ptc2;

	for (i = 0; (*str)[i]; i++) /* search for "||" */
	{
		if ((*str)[i] == '|')
		{
			if ((*str)[++i] == '|')
			{
				i1 = i;
				flag1 = 3;
				ptc1 = *str + i;
			}
			break;
		}
	}

	for (i = 0; (*str)[i]; i++) /* search for "&&" */
	{
		if ((*str)[i] == '&')
		{
			if ((*str)[++i] == '&')
			{
				i2 = i;
				flag2 = 3;
				ptc2 = *str + i;
			}
			break;
		}
	}

	if (!flag1)
	{
		if (flag2)
		{
			(*str) = (*str) + i2 + 1; /* point to the xter afer the token */
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
			(*str) = (*str) + i1 + 1;
			return (1); /* || was found but && wasn't */
		}
		else
		{
			if (ptc1 < ptc2) /* both were found */
			{
				(*str) = (*str) + i1 + 1;
				return (1);
			}
			else
			{
				(*str) = (*str) + i2 + 1;
				return (2);
			}
		}
	}

	return (0);
}
