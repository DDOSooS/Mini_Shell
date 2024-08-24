/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:32:36 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/21 16:32:39 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

static int	check_env_key(char *tmp)
{
	if (tmp == NULL || ft_strcmp(tmp, "_") == 0)
		return (1);
	return (0);
}

int	ft_unset(t_cmd *cmd, t_mshell *shell)
{
	t_cmd	*tmp;

	tmp = cmd->next;
	while (tmp && tmp->arg)
	{
		if (!check_env_key(tmp->arg))
			find_env_rem(&shell->env, tmp->arg);
		tmp = tmp->next;
	}
	return (0);
}
