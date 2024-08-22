/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:34:18 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/21 16:34:19 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

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
