/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:34:18 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/24 10:39:53 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

int	ft_check_unquoted_dollar(char *token)
{
	int	i;

	i = -1;
	while (token[++i])
	{
		if (is_dollar_sign(token[i]) && ft_check_quote(token, i))
			return (0);
		else if (is_dollar_sign(token[i]) && !ft_check_quote(token, i))
			return (1);
	}
	return (0);
}

int	check_tty(void)
{
	if (isatty(STDIN_FILENO))
		return (1);
	return (0);
}

void	print_stderr(char *str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
}

int	export_erorr(char *arg, int status)
{
	write(2, "minishell: export: `", ft_strlen("minishell: export: `"));
	write(2, arg, ft_strlen(arg));
	write(2, "': not a valid identifier\n", 27);
	return (status);
}

void	print_file_error(char *name, char *error)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd("\n", 2);
}
