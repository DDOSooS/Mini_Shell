/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:34:50 by mkartit           #+#    #+#             */
/*   Updated: 2024/08/21 16:34:51 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

static int	match_star(char *regexp, char *text)
{
	while (*text != 0)
	{
		if (match_symbol(regexp + 1, text))
			return (1);
		text += 1;
	}
	if (*text == 0 && *(regexp + 1) == 0)
		return (1);
	return (0);
}

int	match_symbol(char *regexp, char *text)
{
	if ((*regexp == 0 && *text == 0) || (*regexp == 0 && *(regexp - 1) == '*'))
		return (1);
	if (*regexp == '*')
		return (match_star(consume_extra(regexp, '*'), text));
	if (*regexp && *text == *regexp)
		return (match_symbol(regexp + 1, text + 1));
	return (0);
}

int	match_exp(char *regexp, char *text)
{
	if (regexp == 0 || *regexp == 0 || text == 0 || *text == 0)
		return (0);
	if (*text == '.' && *regexp != *text)
		return (0);
	else if (match_symbol(regexp, text))
		return (1);
	return (0);
}

int	check_exp(char *file_name, char *exp)
{
	if (match_exp(exp, file_name))
		return (1);
	return (0);
}

int	open_dir(char *directory, DIR **dir)
{
	if (directory == 0)
		*dir = opendir(".");
	else
		*dir = opendir(directory);
	if (*dir == 0)
		return (1);
	return (0);
}
