/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:32:22 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/21 16:32:23 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	ft_env(t_cmd *cmd, t_mshell *shell)
{
	t_env	*tmp;

	(void)cmd;
	tmp = shell->env;
	while (tmp)
	{
		if (tmp->value && tmp->is_exported == 1)
		{
			if (ft_strcmp(tmp->key, "_") != 0)
				printf("%s=%s\n", tmp->key, tmp->value);
		}
		tmp = tmp->next;
	}
	printf("%s\n", "_=/user/bin/env");
	return (0);
}
