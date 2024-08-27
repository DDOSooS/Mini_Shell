/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:34:43 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/21 16:34:45 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

static void	append_slash(char **file_list)
{
	char	*tmp;

	tmp = ft_strjoin(*file_list, "/");
	free(*file_list);
	*file_list = tmp;
}

void	fill_list(char *directory, char *exp, char **file_list, int dir_flag)
{
	int				i;
	DIR				*dir;
	struct dirent	*file_entry;

	if (open_dir(directory, &dir))
		return ;
	i = 0;
	file_entry = readdir(dir);
	while (file_entry)
	{
		if (check_valid(file_entry, dir_flag, exp))
		{
			if (directory == 0)
				file_list[i] = ft_strdup(file_entry->d_name);
			else
				file_list[i] = ft_strjoin(directory, file_entry->d_name);
			if (dir_flag)
				append_slash(file_list + i);
			i += 1;
		}
		file_entry = readdir(dir);
	}
	closedir(dir);
}

void	expand_wild(char ***args, int i)
{
	char	**file_list;
	char	**tmp;
	char	**current_args;

	if (!args || !*args || !(*args)[i])
		return ;
	current_args = *args;
	get_list_files(current_args[i], &file_list);
	tmp = list_join(current_args, file_list, i);
	if (tmp == NULL)
		return ;
	if (current_args[i])
		free(current_args[i]);
	free(file_list);
	free(current_args);
	*args = tmp;
}

char	**star_expansion(t_cmd *cmd)
{
	int		i;
	char	**cmd_args;

	if (cmd == NULL)
		return (NULL);
	i = 1;
	cmd_args = cmd_args_getter(cmd);
	while (cmd_args[i])
	{
		if (find_char(cmd_args[i], '*') != -1)
			expand_wild(&cmd_args, i);
		i++;
	}
	return (cmd_args);
}

void	astrict_exp(t_cmd **cmd)
{
	char	**args;
	int		i;

	i = 0;
	args = NULL;
	if (cmd == NULL || *cmd == NULL)
		return ;
	args = star_expansion(*cmd);
	ft_free_cmds(*cmd);
	*cmd = NULL;
	while (args && args[i])
	{
		ft_add_cmd(cmd, args[i]);
		i++;
	}
	free_func(args);
}
