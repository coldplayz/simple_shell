#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "main.h"
#define ps(x) (printf("%s\n", (x)))
#define pd(x) (printf("%d\n", (x)))
#define plu(x) (printf("%lu\n", (x)))

int rellocate_alias_val(const char *name, const char *val,
		int edit_idx, int cflag, char ***alias);

int edit_alias_val(char *alias, const char *value, int edit_idx);


/**
 * upd_alias_val - sets a new environment variable, or updates an existing one.
 * @sarr: the array of strings containing command-line arguments to execute.
 * @alias: address of the array of strings representing the shell's aliases.
 *
 * Return: always 1 for the shell's loop to continue.
 */
int upd_alias_val(char **sarr, char ***alias)
{
	int i, j, flag = 0, len;

	len = _strlen(sarr[0]);
	for (i = 0; (*alias)[i]; i++)
	{
		for (j = 0; j < len; j++)
		{
			if (sarr[0][j] == (*alias)[i][j])
			{
				if (((*alias)[i][j + 1] == '='))
				{
				/* if xters match */
				flag = 1;
				}
			}
			else
			{
				/* otherwise leave this loop and go to the next env variable */
				flag = 0;
				break;
			}
		}
		if (flag)
		{
			/* if name is found */
			if (_strlen((*alias)[i] + j + 1) >= _strlen(sarr[1]))
				return (edit_alias_val((*alias)[i], sarr[1], (j + 1)));
			else
				return (rellocate_alias_val(sarr[0], sarr[1], i, 1, alias));
		}
	}
	/* if name is not found */
	return (rellocate_alias_val(sarr[0], sarr[1], -1, 2, alias));
}




/**
 * rellocate_alias_val - creates a new buffer for storing the alias array.
 * @name: alias name.
 * @val: alias value.
 * @edit_idx: the index for the variable
 * to update, if cflag indicates overwrite.
 * @cflag: an int flag indicating whether to overwrite (1),
 * or create a new entry (2) in the environment list.
 * @alias: address of shell's alias list.
 *
 * Description: this function is an extension of upd_alias_val(),
 * and moves alias to a new dynamically allocated memory space.
 * Helper:
 * 1. handle_dpmall()
 * 2. strconcatl()
 * 3. strdup2()
 * Return: always 1 for the shell's loop to continue.
 */
int rellocate_alias_val(const char *name, const char *val,
		int edit_idx, int cflag, char ***alias)
{
	int i = 0, env_len;
	char **buff;

	env_len = str_arrlen(*alias);
	switch (cflag)
	{
		case 1:
			/* for overwrite */
			buff = malloc(sizeof(char *) * (env_len + 1));
			if (handle_dpmall(buff))
				return (1);
			for (i = 0; i < (env_len); i++)
			{
				if (i == edit_idx)
					buff[i] = strconcatl(3, name, "=", val);
				else
					buff[i] = strdup2((*alias)[i]);
			}
			buff[i] = NULL;
			handle_free("e", 0, *alias);
			break;
		case 2:
			/* for new entry */
			buff = malloc(sizeof(char *) * (env_len + 2));
			if (handle_dpmall(buff))
				return (1);
			for (i = 0; i < env_len; i++)
				buff[i] = strdup2((*alias)[i]);
			buff[i++] = strconcatl(3, name, "=", val);
			buff[i] = NULL;
			handle_free("e", 0, *alias);
	}
	*alias = buff;

	return (1);
}




/**
 * edit_alias_val - edits the value of an alias name.
 * @alias: pointer to the alias name=value string to update.
 * @value: the new value of the alias variable.
 * @edit_idx: the index pointing to the first character of the value to edit.
 *
 * Return: always 1 for the shell's loop to continue.
 */
int edit_alias_val(char *alias, const char *value, int edit_idx)
{
	int i, j, flag = 1,
	    len_env = _strlen(alias), len_val = _strlen((char *)value);

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
				alias[i++] = value[j];
			}
		}
		flag = 0;
		alias[i] = 0;
		return (1);
	}

	return (1);
}
