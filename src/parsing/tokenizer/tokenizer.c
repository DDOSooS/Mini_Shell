/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:14:32 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/28 22:54:58 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"

/*
	@return (char *)
	@Description this function return the type o
	token as an string format for more clarification
	int the parser
*/

char	*ft_get_token_type(char *token)
{
	if (is_operator(token[0]) && g_mshell.flag)
		return ("T_OPERATOR");
	if (is_or_operator(token) && g_mshell.flag)
		return ("T_OPERATOR");
	else if (is_pipe(token[0]) && g_mshell.flag)
		return ("T_PIPE");
	else if (is_l_parenthise(token[0]) && g_mshell.flag)
		return ("T_L_PARENTHISE");
	else if (is_r_parenthise(token[0]) && g_mshell.flag)
		return ("T_R_PARENTHISE");
	else if (is_append(token))
		return ("T_APPEND_RED");
	else if (is_herdoc(token))
		return ("T_HERDOC");
	else if (in_redirection(token[0]))
		return ("T_IN_REDIRECTION");
	else if (out_redirection(token[0]))
		return ("T_OUT_REDIRECTION");
	return ("T_WORD");
}

void	ft_add_token_back(t_token **root, t_token *new)
{
	t_token	*tmp;

	tmp = *root;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->previous = tmp;
}

t_token	*ft_new_token(t_token **tokens, char *token, char *cmd_line)
{
	t_token	*new;
	int		type;

	type = ft_get_token_id(token);
	if (type == 7 && g_mshell.n_herdoc == 16)
	{
		ft_putstr_fd("minishell: maximum here-document count exceeded\n", 2);
		ft_free_tokens(tokens);
		return (free(token), free(cmd_line), free_gvar(0), exit(2), NULL);
	}
	else if (type == 7)
		g_mshell.n_herdoc++;
	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = token;
	new->type = ft_get_token_type(token);
	new->type_id = type;
	new->is_exported = 0;
	new->next = NULL;
	new->previous = NULL;
	return (new);
}

int	ft_add_token(t_token **tokens, char *cmd_line, int start, int end)
{
	char	*token;
	t_token	*new;

	if (start == end)
		token = ft_substr(cmd_line, start, 1);
	else
		token = ft_substr(cmd_line, start, end - start + 1);
	if (!token)
		return (0);
	new = ft_new_token(tokens, token, cmd_line);
	if (!new)
		return (0);
	if (!*tokens)
	{
		*tokens = new;
		return (1);
	}
	ft_add_token_back(tokens, new);
	return (1);
}

t_token	*ft_tokinizer(char *cmd)
{
	t_token	*tokens;

	int (i), (start);
	start = -1;
	i = -1;
	tokens = NULL;
	while (cmd[++i])
	{
		if (is_white_space(cmd[i]) && ft_check_opened_token(cmd, i))
			continue ;
		if (ft_check_double_token(cmd, i))
			(ft_add_token(&tokens, cmd, i, i + 1), i++);
		else if (ft_check_sing_token(cmd, i))
			ft_add_token(&tokens, cmd, i, i);
		else if (check_parenthis_token(cmd, i, start))
			ft_add_token(&tokens, cmd, i, i);
		else if (start == -1)
			start = i;
		if (ft_check_end_token(cmd, i, start))
		{
			ft_add_token(&tokens, cmd, start, i);
			start = -1;
		}
	}
	return (tokens);
}
