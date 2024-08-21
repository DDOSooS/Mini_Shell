#include "../../../includes/mshell.h"

int	find_char(char *str, char c)
{
	int	i;

	if (str == 0)
		return (-1);
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i += 1;
	}
	return (-1);
}

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

/*
void	expand_wild(char ***parsed_line, int i)
{
	char	**file_list;
	char	**tmp;

	if (parsed_line == 0
		|| *parsed_line == 0
		|| **parsed_line == 0)
		return ;
	get_list_filter((*parsed_line)[i], &file_list);
	tmp = list_join(*parsed_line, file_list, i);
	if (tmp != 0)
	{
		free((*parsed_line)[i]);
		free(*parsed_line);
		*parsed_line = tmp;
		free(file_list);
	}
}
*/

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
	if (tmp == 0)
		return ;
	*args = tmp;
}

void	star_expansion(t_cmd *cmd, char ***args)
{
	int		i;
	char	**cmd_args;

	if (cmd == NULL)
		return ;
	i = 1;
	cmd_args = cmd_args_getter(cmd);
	while (cmd_args[i])
	{
		if (find_char(cmd_args[i], '*') != -1)
			expand_wild(&cmd_args, i);
		i++;
	}
	*args = cmd_args;
}
