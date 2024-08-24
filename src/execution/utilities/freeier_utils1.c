/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeier_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 11:23:52 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/24 19:31:34 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

void	ft_free_in_file(t_infile *infile)
{
	t_infile	*tmp;

	while (infile)
	{
		tmp = infile->next;
		if (infile->filename)
			free(infile->filename);
		free(infile);
		infile = tmp;
	}
}
