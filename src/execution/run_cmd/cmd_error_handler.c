/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_error_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:33:39 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/21 16:33:40 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

static int	is_directory(char *path)
{
	struct stat	sb;

	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
		return (1);
	return (0);
}

static int	other_cases(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "") == 0 || (ft_strlen(cmd) == 2 && cmd[0] == '"'
			&& cmd[1] == '"'))
	{
		write(2, "minishell: '' : command not found\n", 34);
		return (127);
	}
	if (ft_strcmp(cmd, ".") == 0)
	{
		write(2, "minishell: .: filename argument required\n", 41);
		write(2, ".: usage: . filename [arguments]\n", 33);
		return (2);
	}
	if (ft_strcmp(cmd, "..") == 0)
	{
		write(2, "minishell: ..: command not found\n", 33);
		return (127);
	}
	return (0);
}

int	check_cmd(char *cmd)
{
	if (cmd && ft_strchr(cmd, '/'))
	{
		if (is_directory(cmd))
		{
			write(2, "minishell: ", 11);
			write(2, cmd, ft_strlen(cmd));
			write(2, ": is a directory\n", 17);
			return (126);
		}
		if (access(cmd, F_OK) == 0)
			return (1);
		else
		{
			write(2, "minishell: ", 11);
			write(2, cmd, ft_strlen(cmd));
			write(2, ": No such file or directory\n", 29);
			return (127);
		}
	}
	return (other_cases(cmd));
}
