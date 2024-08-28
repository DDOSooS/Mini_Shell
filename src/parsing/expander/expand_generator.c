/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_generator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 11:19:40 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/27 22:37:24 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	ft_gen_expanded_unquoted_token(char **s1, char *token)
{
	char	*str;
	char	*tmp;

	int (env_len), (i);
	tmp = NULL;
	i = 0;
	if (token[i + 1] && is_dollar_sign(token[i]) && ft_isdigit(token[i + 1]))
		return (1);
	if (token[i + 1] && (is_dollar_sign(token[i + 1]) || token[i + 1] == '?'))
		return (ft_gen_pid_token(s1, token[i + 1]), 1);
	else if (ft_check_sing_dollor(token, i))
	{
		*s1 = ft_strcat_char(*s1, token[i]);
		return (0);
	}
	while (token[++i] && ft_isalnum(token[i + 1]))
		;
	str = ft_substr(token, 1, i);
	env_len = ft_check_env_var(str);
	if (env_len)
		ft_join_expanded_token(s1, tmp, str);
	free(str);
	return (i);
}

int	ft_gen_expanded_quoted_token(char **str, char *token, int flag)
{
	int	i;

	i = 0;
	if (flag)
		*str = ft_strcat_char(*str, token[i]);
	while (token[++i] && ft_check_quote(token, i + 1))
	{
		if (!is_dollar_sign(token[i]))
		{
			*str = ft_strcat_char(*str, token[i]);
			continue ;
		}
		if (is_pid_token(token, i))
		{
			ft_gen_pid_token(str, token[i + 1]);
			i++;
		}
		else if (is_expanded_token(token, i))
			i += ft_gen_search_expanded_token(str, &token[i]);
		else
			*str = ft_strcat_char(*str, token[i]);
	}
	if (flag)
		*str = ft_strcat_char(*str, token[i]);
	return (i);
}

int	ft_gen_unexpanded_token(char **str, char *token, int flag)
{
	int	i;

	i = 0;
	if (flag || check_unclosed_quote(token))
		*str = ft_strcat_char(*str, token[i]);
	while (token[++i] && ft_check_quote(token, i + 1))
		*str = ft_strcat_char(*str, token[i]);
	if (flag)
		*str = ft_strcat_char(*str, token[i]);
	return (i);
}

int	ft_gen_expanded_token(char **str, char *token, int flag)
{
	int	i;

	i = -1;
	while (token[++i])
	{
		if (is_single_quote(token[i]))
		{
			i += ft_gen_unexpanded_token(str, &token[i], flag);
			return (i);
		}
		else if (is_double_quote(token[i]))
		{
			i += ft_gen_expanded_quoted_token(str, &token[i], flag);
			return (i);
		}
		else if (is_dollar_sign(token[i])
			&& token[i + 1] && is_symbol(token[i + 1]))
		{
			i += ft_gen_expanded_unquoted_token(str, &token[i]);
			return (i);
		}
	}
	return (i);
}

int	ft_gen_expanded_arg(char **str, char *token, int flag)
{
	int	i;
	int	counter;

	counter = 0;
	i = -1;
	while (token[++i])
	{
		if (is_quote(token[i]) || (is_dollar_sign(token[i]) && token[i + 1]
				&& (is_symbol(token[i + 1]) || is_quote(token[i + 1]))))
			i += ft_gen_expanded_token(str, &token[i], flag);
		else
		{
			counter++;
			*str = ft_strcat_char(*str, token[i]);
		}
	}
	return (counter);
}
