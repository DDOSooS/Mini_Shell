/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_expnader.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:08:32 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/25 11:07:17 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	ft_count_expanded_line(char *line, int *counter)
{
	char	*tmp;
	char	*sd_tmp;
	int		i;

	i = 0;
	if (line && is_dollar_sign(line[0]))
	{
		(*counter) += ft_count_number_len(g_mshell.pid);
		return (1);
	}
	while (line[i] && ft_isalnum(line[i]))
		i++;
	tmp = ft_substr(line, 0, i);
	sd_tmp = ft_get_env_var(tmp);
	if (sd_tmp)
		*counter += ft_strlen(sd_tmp);
	free(tmp);
	return (i);
}

int	ft_gen_expanded_line(char *line, char **new)
{
	char	*tmp;
	char	*sd_tmp;
	int		i;

	i = 0;
	if (line && is_dollar_sign(line[0]))
	{
		ft_gen_pid_token(new, '$');
		return (1);
	}
	while (line[i] && ft_isalnum(line[i]))
		i++;
	tmp = ft_substr(line, 0, i);
	sd_tmp = ft_get_env_var(tmp);
	if (sd_tmp)
		ft_strcat(*new, sd_tmp);
	free(tmp);
	return (i);
}

int	ft_counte_herdoc_len(char *line)
{
	int	i;
	int	counter;

	counter = 0;
	i = -1;
	while (line[++i])
	{
		if (is_dollar_sign(line[i]) && line[i + 1] && is_symbol(line[i + 1]))
			i += ft_count_expanded_line(&line[i + 1], &counter);
		else
			counter++;
	}
	return (counter);
}

void	ft_gen_expanded_herdoc(char **new, char *line)
{
	int	i;
	int	counter;

	counter = 0;
	i = -1;
	while (line[++i])
	{
		if (is_dollar_sign(line[i]) && line[i + 1] && (ft_isalnum(line[i + 1])
				|| is_dollar_sign(line[i + 1])))
			i += ft_gen_expanded_line(&line[i + 1], new);
		else
			ft_strcat_char(*new, line[i]);
	}
}

int	ft_expand_herdoc_arg(char **arg)
{
	int		len;
	char	*tmp;
	char	*new;

	len = 0;
	tmp = *arg;
	len = ft_counte_herdoc_len(tmp);
	new = malloc(sizeof(char) * len + 1);
	if (!new)
		return (0);
	new[0] = '\0';
	ft_gen_expanded_herdoc(&new, tmp);
	free(*arg);
	*arg = new;
	return (1);
}
