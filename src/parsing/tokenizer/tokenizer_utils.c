/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 10:16:39 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/28 17:16:13 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	ft_check_comments(char *cmd_line)
{
	int	i;

	i = -1;
	if (!cmd_line)
		return (0);
	while (cmd_line[++i])
	{
		if (cmd_line[i] == '#' && !ft_check_quote(cmd_line, i) && (!i
				|| is_white_space(cmd_line[i - 1])))
			return (1);
	}
	return (0);
}

void	ft_handle_comment(char **cmd_line)
{
	char	*new_cli;
	char	*tmp;
	int		i;

	tmp = *cmd_line;
	i = -1;
	while (tmp[++i])
	{
		if (tmp[i] == '#' && !ft_check_quote(tmp, i) && (!i
				|| is_white_space(tmp[i - 1])))
			break ;
	}
	if (tmp[i])
	{
		new_cli = ft_substr(tmp, 0, i);
		free(*cmd_line);
		*cmd_line = new_cli;
	}
}

int	ft_check_end_token(char *cmd, int i, int start)
{
	if ((is_white_space(cmd[i + 1]) && ft_check_opened_token(cmd, i + 1))
		&& start != -1)
		return (1);
	if (((is_tokens(cmd[i + 1]) && !ft_check_quote(cmd, i + 1)
				&& is_closed_parenthise(cmd, i + 1))) && start != -1)
		return (1);
	if (is_r_parenthise(cmd[i + 1]) && check_parenthise_token(cmd, i + 1)
		&& !ft_check_quote(cmd, i + 1))
		return (1);
	if (!cmd[i + 1] && start != -1)
		return (1);
	return (0);
}

int	ft_check_double_token(char *cmd, int i)
{
	return (is_doubled_token(&cmd[i]) && (!ft_check_quote(cmd, i)
			&& is_closed_parenthise(cmd, i)));
}

int	ft_check_sing_token(char *cmd, int i)
{
	return ((is_pipe(cmd[i]) || is_redirection(cmd[i])) && !ft_check_quote(cmd,
			i) && is_closed_parenthise(cmd, i));
}
