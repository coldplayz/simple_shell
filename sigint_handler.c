#include <sys/types.h>
#include <sys/stat.h>
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

/**
 * sigint_handler - handles signals from keyboard
 * interrupts, including EOF (ctrl+D) and ctrl+C signals.
 * @signum: the signal number.
 *
 * Return: prevoius handler used; in this case, itself.
 */
void sigint_handler(int signum)
{
	if (signum == SIGINT)
	{
		exit(EXIT_SUCCESS); /* or other handles */
	}
}
