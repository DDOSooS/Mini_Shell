/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:32:17 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/21 16:32:18 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

static int	echo_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' && arg[i + 1] == 'n')
	{
		i++;
		while (arg[i] == 'n')
			i++;
	}
	if (arg[i] == '\0')
		return (1);
	return (0);
}

static void	print_args(t_cmd *tmp, int flag)
{
	tmp = tmp->next;
	if (flag == 1 && tmp && tmp->next)
		tmp = tmp->next;
	while (tmp && tmp->arg)
	{
		printf("%s", tmp->arg);
		if (tmp->next)
			printf(" ");
		tmp = tmp->next;
	}
}

int	ft_echo(t_cmd *cmd, t_mshell *shell)
{
	t_cmd	*tmp;

	int (flag), (count);
	tmp = cmd;
	flag = 1;
	count = 0;
	UNUSED(shell);
	if (tmp->next && tmp->next->arg)
	{
		while (tmp->next && tmp->next->arg)
		{
			flag = echo_arg(tmp->next->arg);
			if (!flag)
				break ;
			count++;
			tmp = tmp->next;
		}
		print_args(tmp, flag);
	}
	if (count == 0)
		printf("\n");
	return (0);
}
