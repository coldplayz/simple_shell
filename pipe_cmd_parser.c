#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include "main.h"

#define ex (execve("/bin/ls", args, NULL) == (-1))
#define ps(x) (printf("%s\n", (x)))
#define pd(x) (printf("%d\n", (x)))
#define plu(x) (printf("%lu\n", (x)))

char *strtok3(char *str, const char *delim, int quote);


/**
 * pipe_parser - parses commands piped to my shell.
 * @line: the unmodified command-line input
 * @envp: the process' environment.
 */
void pipe_parser(char *line, char **envp)
{
	char *line2, *token, **sarr;
	int i;

	line2 = strdup2(line); /* to free line2 */
	sarr = str_arr(line2, " \n"); /* to free sarr */

	token = strtok3(line, " \n", 0); /* strtok3 leaves line unchanged */
	for (i = 1; sarr[i]; i++)
	{
		token = strtok3(NULL, " \n", 0); /* starts from second word of line */
		if (iscmd(sarr[i], envp))
		{
			*(token - 1) = ';'; /* place ';' before the first xter of a command */
		}
	}

	free(line2);
	free(sarr);
	shstruct(NULL)->quick_exit = 0;
}




/**
 * strtok3 - tokenize a string, str, based on delimiters, delim.
 * @str: the string to tokenize.
 * @delim: delimiter characters.
 * @quote: an int determining whether to apply
 * tokenization to quoted stings (non-zero), or not (0).
 *
 * Description: this version of strtok2 doesn't
 * null-terminate potential tokens. Serves as a helper to pipe_parser.
 * Return: the address of the current extracted token, or NULL.
 */
char *strtok3(char *str, const char *delim, int quote)
{
	char *char1;
	static char *ptc;
	int i, j, n = 0, flag = 1;

	if (str != NULL)
		ptc = str;
	if (*ptc == '\0')
		return (NULL);
	if (delim == NULL)
		return (ptc);
	for (i = 0; 1; i++)
	{
		for (j = 0; delim[j]; j++)
		{
			if (*ptc == delim[j])
			{
				if (n)
				{
					ptc++; /* Point to next character for the next call */
					return (char1);
				}
				flag = 0;
			}
		}
		if (flag)
		{
			if (++n == 1)
				char1 = ptc;
			if ((*ptc == '\'' || *ptc == '\"') && quote)
				find_quote(&ptc, &n); /* prevent application of tokenizaton to quotes */
		}
		flag = 1;
		if ((*ptc == '\0') && n) /*A non-delim char was seen by string end, else...*/
			return (char1);
		else if ((*ptc == '\0') && !n) /*none was seen for this call by string end*/
			return (NULL);
		ptc++;
	}
	return (NULL);
}

/* ====================NOTES=================== */

/* L31: n is greater than 0 only if a */
/* non-delimiter character has been encountered. */
/* This makes it possible to ignore any */
/* leading delim xter and prevent a false termination. */

/* L37: flag is set to 0 only when a delim xter is met, */
/* to prevent entrance into the IF block on L41, which is */
/* only meant for when the current value of ptc is a non-delim xter. */
