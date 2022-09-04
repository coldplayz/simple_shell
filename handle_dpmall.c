#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "main.h"


/**
 * handle_dpmall - handle mallocc return for double pointers.
 * @buff: malloc's return.
 *
 * Return: 0 if successfull malloc, -1 otherwise.
 */
int handle_dpmall(char **buff)
{
	if (!buff)
	{
		perror("rellocate_env:malloc");
		return (-1);
	}

	return (0);
}
