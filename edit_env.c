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
 * edit_env - edits the value of an environment variable.
 * @env: pointer to the environment name=value string to update.
 * @value: the new value of the env variable.
 * @edit_idx: the index pointing to the first character of the value to edit.
 *
 * Return: always 1 for the shell's loop to continue.
 */
int edit_env(char *env, const char *value, int edit_idx)
{
	int i, j, flag = 1, len_env = _strlen(env), len_val = _strlen((char *)value);

	for (i = edit_idx; i <= len_env; i++)
	{
		if (flag)
		{
			for (j = 0; j < len_val; j++)
			{
				if (value[j] == '=')
				{
					fprintf2(STDERR_FILENO, "The character = is in value\n");
					return (1);
				}
				env[i++] = value[j];
			}
		}
		flag = 0;
		env[i] = 0;
	}

	return (1);
}
