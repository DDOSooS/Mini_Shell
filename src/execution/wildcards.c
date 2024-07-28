#include "../../includes/mshell.h"

static int	match_symbol(char *regexp, char *text);

static char	*consume_extra(char *reg, char c)
{
	while (*reg == c && *(reg + 1) == c)
		reg += 1;
	return (reg);
}

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

static int	match_star(char *regexp, char *text)
{
	while (*text != 0)
	{
		if (match_symbol(regexp + 1, text))
			return (1);
		text += 1;
	}
	if (*text == 0 && *(regexp + 1) == 0)
		return (1);
	return (0);
}

static int	match_symbol(char *regexp, char *text)
{
	if ((*regexp == 0 && *text == 0)
		|| (*regexp == 0 && *(regexp - 1) == '*'))
		return (1);
	if (*regexp == '*')
		return (match_star(consume_extra(regexp, '*'), text));
	if (*regexp && *text == *regexp)
		return (match_symbol(regexp + 1, text + 1));
	return (0);
}

int	match_exp(char *regexp, char *text)
{
	if (regexp == 0 || *regexp == 0
		|| text == 0 || *text == 0)
		return (0);
	if (*text == '.' && *regexp != *text)
		return (0);
	else if (match_symbol(regexp, text))
		return (1);
	return (0);
}

int	check_exp(char *file_name, char *exp)
{
	if (match_exp(exp, file_name))
		return (1);
	return (0);
}



int	check_valid(struct dirent *file_entry, int dir_flag, char *exp)
{
	if (dir_flag == 1 && file_entry -> d_type == DT_DIR
		&& check_exp(file_entry -> d_name, exp))
		return (1);
	if (dir_flag == 0 && check_exp(file_entry -> d_name, exp))
		return (1);
	return (0);
}



int	open_dir(char *directory, DIR **dir)
{
	if (directory == 0)
		*dir = opendir(".");
	else
		*dir = opendir(directory);
	if (*dir == 0)
		return (1);
	return (0);
}


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

void	is_dir(char *exp, int *flag)
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

void	append_slash(char **file_list)
{
	char	*tmp;

	tmp = ft_strjoin(*file_list, "/");
	free(*file_list);
	*file_list = tmp;
}


static void	fill_list(char *directory, char *exp,
		char **file_list, int dir_flag)
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
				file_list[i] = ft_strdup(file_entry -> d_name);
			else
				file_list[i] = ft_strjoin(directory, file_entry -> d_name);
			if (dir_flag)
				append_slash(file_list + i);
			i += 1;
		}
		file_entry = readdir(dir);
	}
	closedir(dir);
}



void	get_list_files(char *exp, char ***file_list)
{
	int				quant;
	char			*directory;
	int				dir_exp_flag;

	*file_list = 0;
	directory = separate_and_get_dir(&exp);
	is_dir(exp, &dir_exp_flag);
	quant = count_file(directory, exp, dir_exp_flag);
	if (quant > 0)
		*file_list = ft_calloc(quant + 1, sizeof(char *));
	if (*file_list == 0)
	{
		if (dir_exp_flag)
			exp[ft_strlen(exp)] = '/';
		return ;
	}
	fill_list(directory, exp, *file_list, dir_exp_flag);
	if (dir_exp_flag)
		exp[ft_strlen(exp)] = '/';
	free(directory);
}

static int	list_size(char **list)
{
	int	i;

	i = 0;
	while (list[i])
		i += 1;
	return (i);
}

static void	move_pointer(char **dest, char **src, int *b_i)
{
	while (*src)
	{
		dest[*b_i] = *src;
		*b_i += 1;
		src += 1;
	}
}


static char	**list_join(char **list_one, char **list_two, int i)
{
	int		b_i;
	char	**buff;

	if (list_one == 0 || list_two == 0)
		return (0);
	buff = ft_calloc(list_size(list_one) + list_size(list_two) + 1,
			sizeof(char *));
	if (buff == 0)
		return (0);
	b_i = 0;
	while (*list_one)
	{
		if (b_i == i)
			move_pointer(buff, list_two, &b_i);
		else
		{
			buff[b_i] = *list_one;
			b_i += 1;
		}
		list_one += 1;
	}
	return (buff);
}


// void	expand_wild(char ***parsed_line, int i)
// {
// 	char	**file_list;
// 	char	**tmp;

// 	if (parsed_line == 0
// 		|| *parsed_line == 0
// 		|| **parsed_line == 0)
// 		return ;
// 	get_list_filter((*parsed_line)[i], &file_list);
// 	tmp = list_join(*parsed_line, file_list, i);
// 	if (tmp != 0)
// 	{
// 		free((*parsed_line)[i]);
// 		free(*parsed_line);
// 		*parsed_line = tmp;
// 		free(file_list);
// 	}
// }

void expand_wild(char ***args, int i)
{
    char **file_list;
    char **tmp;
    char **current_args;

    if (!args || !*args || !(*args)[i])
        return;
    current_args = *args;
    get_list_files(current_args[i], &file_list);
    tmp = list_join(current_args, file_list, i);
    if (tmp == 0)
        return;
    *args = tmp;
}

void star_expansion(t_cmd *cmd, char ***args)
{
    int i;
    char **cmd_args;

    if (cmd == NULL)
        return;
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