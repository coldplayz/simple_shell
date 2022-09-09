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

#define ps(x) (printf("%s\n", (x)))
#define pd(x) (printf("%ld\n", (x)))
#define plu(x) (printf("%lu\n", (x)))


/**
 * is_binsh - determines whether a file is a binary exec. or script file.
 * @filename: name of the file whose type to determine.
 *
 * Return: 1 on success; 0 otherwise.
 */
int is_binsh(char *filename)
{
	int i, fdrd;
	char buff[5] = "\0\0\0\0\0", sh[3] = "#!";
	int flag = 1, elf[4] = {0x7f, 0x45, 0x4c, 0x46};

	fdrd = open(filename, O_RDONLY);
	if (fdrd == -1)
	{
		perror("open");
		return (1);
	}

	if (read(fdrd, buff, 4) == -1)
	{
		perror("read");
		return (2);
	}

	for (i = 0; buff[i]; i++)
	{
		if (buff[i] != elf[i])
		{
			flag = 0;
			break;
		}
	}

	if (flag)
	{
		return (1);
	}

	close(fdrd);

	return (isscript(buff, sh));
}
