/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:27:42 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/27 18:30:50 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	is_first_left_parenthise(char *cmd, int index)
{
	int	i;

	if (!cmd)
		return (0);
	i = 0;
	while (cmd[i])
	{
		if (is_l_parenthise(cmd[i]) && !ft_check_quote(cmd, i))
		{
			if (i == index)
				return (1);
			return (0);
		}
		i++;
	}
	return (1);
}

int	is_last_right_parentise(char *cmd, int index)
{
	int	i;

	i = ft_strlen(cmd);
	while (--i >= 0)
	{
		if (is_r_parenthise(cmd[i]) && !ft_check_quote(cmd, i))
		{
			if (index == i)
				return (1);
			return (0);
		}
	}
	return (1);
}

int	check_left_p_token(char *token, int index)
{
	if (!index)
		return (1);
	while (index - 1 >= 0 && token[--index])
	{
		if ((is_pipe(token[index]) || is_operator(token[index]))
			&& !ft_check_quote(token, index))
			break ;
		if (is_l_parenthise(token[index]) && !ft_check_quote(token, index))
			return (0);
	}
	return (1);
}

int	check_r_parenthis(char *token, int i)
{
	while (token[++i])
	{
		if ((is_pipe(token[i]) || is_operator(token[i]))
			&& !ft_check_quote(token, i))
			break ;
		if (is_r_parenthise(token[i]) && !ft_check_quote(token, i))
			return (0);
	}
	return (1);
}

int	check_right_p_token(char *token, int index)
{
	int	i;
	int	r_counter;
	int	l_counter;

	r_counter = 1;
	l_counter = 0;
	i = index;
	if (is_last_right_parentise(token, index))
		return (1);
	i = index;
	while (--i >= 0 && token[i])
	{
		if ((is_pipe(token[i]) || is_operator(token[i]))
			&& !ft_check_quote(token, i))
			break ;
		if (is_r_parenthise(token[i]) && !ft_check_quote(token, i))
			r_counter++;
		if (is_l_parenthise(token[i]) && !ft_check_quote(token, i))
			l_counter++;
	}
	if (l_counter == r_counter)
		if (check_r_parenthis(token, index))
			return (1);
	return (0);
}
