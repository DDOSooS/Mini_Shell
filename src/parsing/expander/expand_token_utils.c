/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 20:26:08 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/22 20:26:23 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/mshell.h"


void ft_reset_token(t_token **token, t_token **root)
{
    if (*token == *root)
        *root = (*token)->next;
    if (!(*token)->previous && (*token)->next)
    {
        (*token)->next->previous = NULL;
            
    }
    else if ((*token)->previous && (*token)->next)
    {
        (*token)->previous->next = (*token)->next;
        (*token)->next->previous = (*token)->previous;
    }
    else if ((*token)->previous && !(*token)->next)
        (*token)->previous->next = NULL;
}


void    ft_expand_token_helper(int *flag, t_token **token)
{
    if ((*token)->value && ft_check_dollar((*token)->value))
    {
        *flag = 1;
        (*token)->is_exported = 1;
    }
    ft_expand_arg(&(*token)->value);
}

void ft_delet_token(t_token **token, t_token **root)
{
    t_token *sd_tmp;

    sd_tmp = NULL;
    sd_tmp = (*token)->next;
    ft_reset_token(token, root);
    if ((*token)->value)
        free((*token)->value);
    free((*token));
    (*token) = NULL;
    (*token) = sd_tmp;
}
