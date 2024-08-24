/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeier_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 11:14:05 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/24 19:31:40 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

void	ft_free_cmd_var(void)
{
	ft_free_tokens(&g_mshell.token);
	ft_free_herdoc(&g_mshell.herdocs);
	ft_free_tree(&g_mshell.cmd_tree);
}

void	ft_free_tokens(t_token **tokens)
{
	t_token	*tmp;

	if (!tokens || !*tokens)
		return ;
	while (*tokens)
	{
		tmp = (*tokens)->next;
		if ((*tokens)->value)
			free((*tokens)->value);
		free(*tokens);
		*tokens = tmp;
	}
	*tokens = NULL;
}

void	ft_free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;

	while (cmds)
	{
		tmp = cmds->next;
		free(cmds->arg);
		free(cmds);
		cmds = tmp;
	}
	free(cmds);
}

void	ft_free_out_files(t_outfile *outfile)
{
	t_outfile	*tmp;

	while (outfile)
	{
		tmp = outfile->next;
		if (outfile->filename)
			free(outfile->filename);
		free(outfile);
		outfile = tmp;
	}
}

void	ft_free_redirections(t_redirection *redirection)
{
	if (redirection)
	{
		if (redirection->in_file)
			ft_free_in_file(redirection->in_file);
		if (redirection->out_file)
			ft_free_out_files(redirection->out_file);
		free(redirection);
	}
}
