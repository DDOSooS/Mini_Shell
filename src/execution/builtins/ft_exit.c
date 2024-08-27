/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:32:29 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/21 16:32:30 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

long long int	ft_atoll(char *str)
{
	long long int	status;
	int				sign;
	int				i;

	status = 0;
	sign = 1;
	i = 0;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign *= -1;
	while (str && str[i] && str[i] >= '0' && str[i] <= '9')
	{
		if (status > LLONG_MAX / 10 || (status == LLONG_MAX / 10
				&& (str[i] - '0') > LLONG_MAX % 10))
		{
			if (sign == -1 && status == LLONG_MAX / 10
				&& str[i] == '8' && str[i + 1] == '\0')
				return (0);
			if (sign == 1 || (sign == -1 && status > LLONG_MAX / 10))
				return (1);
			return (1);
		}
		status = status * 10 + (str[i++] - '0');
	}
	return (0);
}

static int	exit_value_check(char *arg)
{
	int				i;
	int				status;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (1);
		i++;
	}
	status = ft_atoll(arg);
	if (status)
		return (1);
	return (0);
}

static int	exit_handler(char *arg)
{
	int		exit_value;
	char	*buffer;

	buffer = NULL;
	if (exit_value_check(arg))
	{
		buffer = ft_strjoin("minishell: exit: ", arg);
		write(2, buffer, ft_strlen(buffer));
		write(2, ": numeric argument required\n", 28);
		free(buffer);
		free_gvar(1);
		return (2);
	}
	exit_value = ft_atoi(arg);
	if (exit_value < 0)
		exit_value = 256 + exit_value;
	free_gvar(1);
	return (exit_value);
}

int	ft_exit(t_cmd *cmd, t_mshell *shell)
{
	t_cmd	*tmp;

	tmp = NULL;
	(void)shell;
	if (cmd->next != NULL)
		tmp = cmd->next;
	if (check_tty())
		write(2, "exit\n", 5);
	if (tmp && tmp->next != NULL && !exit_value_check(tmp->arg))
	{
		print_stderr("exit: too many arguments");
		return (1);
	}
	if (tmp && tmp->arg)
		exit(exit_handler(tmp->arg));
	else
	{
		free_gvar(1);
		exit(g_mshell.exit_value);
	}
	return (0);
}
