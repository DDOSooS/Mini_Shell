/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:14:32 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/19 18:33:25 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../../includes/mshell.h"

/*
    @return (char *)
    @Description this function return the type of token as an string format for more clarification int the parser
*/

char * ft_get_token_type(char *token)
{
    if (is_operator(token[0]))
        return ("T_OPERATOR");
    if (is_or_operator(token))
        return ("T_OPERATOR");
    else if (is_pipe(token[0]))
        return ("T_PIPE");
    else if (is_l_parenthise(token[0]))
        return ("T_L_PARENTHISE");
    else if (is_r_parenthise(token[0]))
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
 


void ft_add_token_back(t_token **root, t_token *new)
{
    t_token *tmp;

    tmp = *root;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
    new->previous = tmp;
}

t_token *ft_new_token(char *token)
{ 
    t_token *new;
    int type;

    type = ftGetTokenId(token);
    if (type == 7 && g_mshell.n_herdoc == 16)
    {
        free(token);
        ft_putstr_fd("maximum here-document count exceeded\n", 2);
        return (NULL);
    }
    else if (type == 7)
        g_mshell.n_herdoc++;   
    new = malloc(sizeof(t_token));
    if (!new) 
        return NULL;
    new->value = token;
    new->type = ft_get_token_type(token);
    new->typeId = type;
    new->is_exported = 0;
    new->next = NULL;
    new->previous = NULL;
    return new;
}

int ft_add_token(t_token **tokens, char *cmd_line, int start, int end)
{
    char    *token;
    t_token *new;
    
    if (start == end)
        token = ft_substr(cmd_line, start, 1);
    else    
        token = ft_substr(cmd_line, start, end - start + 1);    
    if (! token)
        return (0);
    new = ft_new_token(token);
    if (!new)
        return (0);
    if(!*tokens)
    {
        *tokens = new;
        return (1);
    }
    ft_add_token_back(tokens, new);
    return (1);
}
//to be resized

t_token *ft_tokinizer(char *cmd)
{
    t_token *tokens;
    int     i;
    int     start;

    start = -1;
    i = -1;
    tokens = NULL;
    while (cmd[++i])
    {
        if (is_whites_space(cmd[i]) && ft_check_opened_token(cmd, i))
            continue;
        if (is_doubled_token(&cmd[i]) && (!ft_check_quote(cmd, i) && is_closed_parenthise(cmd, i)))
        {
            if (! ft_add_token(&tokens, cmd, i, i + 1))
                return (ft_free_tokens(&tokens), NULL);
            i++;
        }
        else if ((is_pipe(cmd[i]) || is_redirection(cmd[i])) && !ft_check_quote(cmd, i) && is_closed_parenthise(cmd, i))
        {
            if (!ft_add_token(&tokens, cmd, i, i))
                return (ft_free_tokens(&tokens), NULL);
        }
        else if ((is_parenthise(cmd[i]) && (!ft_check_quote(cmd, i))))
        {
            if (!ft_add_token(&tokens, cmd, i, i))
                return (ft_free_tokens(&tokens), NULL);
            
        }
        else if (start == -1)
            start = i;
        if (((is_whites_space(cmd[i + 1]) && ft_check_opened_token(cmd, i+1))
            || (is_r_parenthise(cmd[i + 1]) && !ft_check_quote(cmd, i + 1)) || !cmd[i + 1]
            || (is_tokens(cmd[i + 1]) && !ft_check_quote(cmd, i + 1) && is_closed_parenthise(cmd, i + 1))) && start != -1)
        {
            if (!ft_add_token(&tokens, cmd, start, i))
                return (ft_free_tokens(&tokens), NULL);
            start = -1;
        }
    } 
    return (tokens);
} 