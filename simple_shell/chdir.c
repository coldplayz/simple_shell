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

/**
 * cd2 - changes the shell's current working directory.
 * @sarr: the array of strings containing command-line arguments to execute..
 * @envp: the process's environment
 * @status: address of int storing the shell's exit/return status.
 * @free2: address of an int indicating
 * whether envp has been previously rellocated.
 *
 * Return: always 1 for the shell's loop to continue.
 */
int cd2(char **sarr, char ***envp, int *status, int *free2)
{
	int free_newpwd = 0;
	char *ptc, *oldpwd, *newpwd = sarr[1];
	char *pptc[4] = {"placeholder", "OLDPWD", NULL, NULL};
	char *pptc2[4] = {"placeholder", "PWD", NULL, NULL};

	pptc2[2] = newpwd;
	if (sarr[1] == NULL) /* no argument to cd */
	{
		newpwd = getenv3("HOME", *envp);
		if (!newpwd)
			return (1);
		ptc = newpwd;
		pptc2[2] = newpwd;
	}
	else if (sarr[1][0] == '-' && sarr[1][1] == 0)
	{
		if (getenv3("OLDPWD", *envp))
			newpwd = strdup2(getenv3("OLDPWD", *envp)); /* independent copy of OLDPWD */
		else if (getenv3("PWD", *envp))
			newpwd = strdup2(getenv3("PWD", *envp));
		else
			return (1);
		free_newpwd = 1;
		ptc = newpwd;
		pptc2[2] = newpwd;
		fprintf2(STDOUT_FILENO, "%s\n", newpwd);
	}
	else
	{
		ptc = sarr[1];
	}
	oldpwd = getenv3("PWD", *envp);
	pptc[2] = oldpwd;
	if (chdir(ptc) == -1)
	{
		fprintf2(STDERR_FILENO, "%s: %d: cd: can't cd to %s\n",
				shstruct(NULL)->name, shstruct(NULL)->loop_cnt, ptc);
		return (1);
	}
	else
	{
		setenv2(pptc, envp, status, free2); /* set OLDPWD */
		setenv2(pptc2, envp, status, free2); /* set PWD */
		if (free_newpwd)
			free(newpwd);
	}
	return (1);
}
