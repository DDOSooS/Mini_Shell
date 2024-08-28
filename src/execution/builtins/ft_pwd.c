/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:32:43 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/21 16:32:44 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

static int	print_pwd(t_mshell *shell)
{
	char	*cwd;
	char	buffer[1024];

	if (find_env(shell->env, "PWD") && find_env(shell->env, "PWD")->value)
		cwd = find_env(shell->env, "PWD")->value;
	else
	{
		cwd = getcwd(buffer, 1024);
		if (!cwd)
		{
			perror("getcwd");
			return (1);
		}
	}
	printf("%s\n", cwd);
	return (0);
}

int	ft_pwd(t_cmd *cmd, t_mshell *shell)
{
	(void)cmd;
	return (print_pwd(shell));
}
