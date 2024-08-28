/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_list_files.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:34:36 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/28 11:27:33 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

static int	count_file(char *directory, char *exp, int dir_flag)
{
	int				i;
	DIR				*dir;
	struct dirent	*file_entry;

	i = 0;
	if (open_dir(directory, &dir))
		return (-1);
	file_entry = readdir(dir);
	while (file_entry)
	{
		i += check_valid(file_entry, dir_flag, exp);
		file_entry = readdir(dir);
	}
	closedir(dir);
	return (i);
}

static void	is_dir(char *exp, int *flag)
{
	int	i;

	*flag = 0;
	i = ft_strlen(exp) - 1;
	while (i > -1)
	{
		if (exp[i] == '/')
			*flag = 1;
		if (*flag == 1 && exp[i] != '/')
			exp[i + 1] = 0;
		if (exp[i] != '/')
			break ;
		i -= 1;
	}
}

static char	*separate_and_get_dir(char **exp)
{
	char	*dir;
	char	c;
	int		i;
	int		flag;

	i = ft_strlen(*exp) - 1;
	flag = 0;
	while (i > -1)
	{
		if ((*exp)[i] == '*')
			flag = 1;
		if (flag && (*exp)[i] == '/')
			break ;
		i -= 1;
	}
	if (i > -1)
	{
		c = (*exp)[i + 1];
		(*exp)[i + 1] = 0;
		dir = ft_strdup(*exp);
		(*exp)[i + 1] = c;
		*exp = &((*exp)[i + 1]);
		return (dir);
	}
	return (0);
}

int	check_valid(struct dirent *file_entry, int dir_flag, char *exp)
{
	if (dir_flag == 1 && file_entry->d_type == DT_DIR
		&& check_exp(file_entry->d_name, exp))
		return (1);
	if (dir_flag == 0 && check_exp(file_entry->d_name, exp))
		return (1);
	return (0);
}

void	get_list_files(char *exp, char ***file_list)
{
	int		counter;
	char	*directory;
	int		dir_exp_flag;

	*file_list = 0;
	dir_exp_flag = 0;
	directory = separate_and_get_dir(&exp);
	is_dir(exp, &dir_exp_flag);
	counter = count_file(directory, exp, dir_exp_flag);
	if (counter > 0)
		*file_list = ft_calloc(counter + 1, sizeof(char *));
	if (*file_list == 0)
	{
		if (dir_exp_flag)
			exp[ft_strlen(exp)] = '/';
		if (directory)
			free(directory);
		return ;
	}
	fill_list(directory, exp, *file_list, dir_exp_flag);
	if (dir_exp_flag)
		exp[ft_strlen(exp)] = '/';
	free(directory);
}
