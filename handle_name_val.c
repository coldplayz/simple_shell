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
 * handle_name_val - checks for NULL names and/or values.
 * @name: variable name.
 * @val: variable value.
 *
 * Return: an int representing the outcome of the checks.
 */
int handle_name_val(const char *name, const char *val)
{
	int i;

	if (!name)
	{
		fprintf(stderr, "Invalid name\n");
		return (1);
	}
	for (i = 0; name[i]; i++)
	{
		if ((name[i] == '=') || (name[0] == 0))
		{
			fprintf(stderr, "Invalid name\n");
			return (1);
		}
	}

	if (!val)
	{
		fprintf(stderr, "Invalid value\n");
		return (1);
	}

	return (0);
}
