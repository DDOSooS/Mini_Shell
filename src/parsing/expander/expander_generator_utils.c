/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_generator_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 11:25:20 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/22 12:02:48 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

pid_t	get_pid(void)
{
	pid_t	pid;

	pid = fork();
	if (!pid)
		exit(0);
	else
		wait(NULL);
	return (pid - 2);
}

void	ft_join_expanded_token(char **joined_str, char *tmp, char *str)
{
	tmp = ft_get_env_var(str);
	*joined_str = ft_strcat(*joined_str, tmp);
}

char	*ft_get_env_var(char *str)
{
	t_env	*tmp;

	tmp = g_mshell.env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, str))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

int	ft_gen_pid_token(char **str, char token)
{
	char	*tmp;
	int		number;

	if (token == '?')
		number = g_mshell.exit_value;
	else
		number = g_mshell.pid;
	tmp = ft_itoa(number);
	if (!tmp)
		return (0);
	*str = ft_strcat(*str, tmp);
	free(tmp);
	return (1);
}

int	ft_gen_search_expanded_token(char **s1, char *token)
{
	int		i;
	char	*str;
	int		env_len;
	char	*tmp;

	i = 0;
	while (token[++i] && !is_quote(token[i + 1]) && ft_isalnum(token[i + 1]))
		;
	str = ft_substr(token, 1, i);
	env_len = ft_check_env_var(str);
	if (env_len)
	{
		tmp = ft_get_env_var(str);
		if (tmp)
			*s1 = ft_strcat(*s1, tmp);
	}
	free(str);
	return (i);
}
