/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:14:32 by aghergho          #+#    #+#             */
/*   Updated: 2024/05/29 16:45:54 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/mshell.h"

int is_or_operator(char *token)
{
    if (is_pipe(token[0]) && token[1] && is_pipe(token[1]))
        return (1);
    return (0);
}

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
 
/*
    @return (int)
    @Description this function return the type id of a token that well be used in the parser 
*/

int ftGetTokenId(char *token)
{
    if (is_operator(token[0]))
        return (3);
    if (is_or_operator(token))
        return (2);
    else if (is_pipe(token[0]))
        return (1);
    else if (is_l_parenthise(token[0]))
        return (4);
    else if (is_r_parenthise(token[0]))
        return (5);
    else if (is_append(token))
        return (6);
    else if (is_herdoc(token))
        return (7);
    else if (in_redirection(token[0]))
        return (8);
    else if (out_redirection(token[0]))
        return (9);
    return (0);       
}

/*
    @return (t_token)
    @Description this function create a new token 
*/

t_token *ft_new_token(char *token)
{
    t_token *new;
    
    new = malloc(sizeof(t_token));
    if (!new) 
        return NULL;
    new->value = token;
    new->type = ft_get_token_type(token);
    new->typeId = ftGetTokenId(token);
    new->next = NULL;
    return new;
}

/*
    @return (int)
    @Description this function is responsible for adding  a token 
*/

int ft_add_token(t_token **tokens, char *cmd_line, int start, int end)
{
    t_token *tmp;
    char    *token;
    t_token *new;
    
    if (start == end)
        token =ft_substr(cmd_line, start, 1);
    else    
        token =ft_substr(cmd_line, start, end - start +1);    
    if (! token)
        return (0);
    new = ft_new_token(token);
    if (!new)
        return (0);
    tmp = *tokens;
    if(!*tokens)
    {
        *tokens = new;
        return (1);
    }
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
    return (1);
}

int is_tokens(char c)
{
    if (is_redirection(c) || is_l_parenthise(c) || is_r_parenthise(c) || is_pipe(c) || is_operator(c))
        return (1);
    return (0);
}

int is_parenthise(char c)
{
    if (is_l_parenthise(c) || is_r_parenthise(c))
        return (1);
    return (0);
}

int ft_check_opened_token(char *cmd, int len)
{
    if (!ft_check_quote(cmd, len) && is_closed_parenthise(cmd, len) )
        return (1);
    return (0);
}

int ft_check_l_parenthise(char *cmd, int i)
{
    while (cmd[i])
    {
        if (is_whites_space(cmd[i]) && !ft_check_quote(cmd, i))
        {
            i++;
            continue;
        }
        if (is_l_parenthise(cmd[i]) && !ft_check_quote(cmd, i))
            return (1);
        i++;
    }
    return (0);
}

int ft_check_r_parenthise(char *cmd, int i)
{
    if (!cmd[i])
        return (0);
    while (cmd[i])
    {
        if (is_whites_space(cmd[i]) && !ft_check_quote(cmd, i))
        {
            i++;
            continue;
        }        
        if (is_r_parenthise(cmd[i]) && !ft_check_quote(cmd, i))
            return (1);
        i++;
    }
    return (0);
}

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
        if ((is_l_parenthise(cmd[i]) && ft_check_l_parenthise(cmd, i + 1)) || (is_r_parenthise(cmd[i]) && ft_check_r_parenthise(cmd, i + 1)))
            continue;
        if (is_doubled_token(&cmd[i]) && (!ft_check_quote(cmd, i) && is_closed_parenthise(cmd, i)))
        {
            ft_add_token(&tokens, cmd, i, i + 1);
            i++;
        }
        else if ((is_pipe(cmd[i]) || is_redirection(cmd[i])) && !ft_check_quote(cmd, i) && is_closed_parenthise(cmd, i))
            ft_add_token(&tokens, cmd, i, i);
        else if ((is_parenthise(cmd[i]) && (!ft_check_quote(cmd, i))))
            ft_add_token(&tokens, cmd, i, i);
        else if (start == -1)
            start = i;

         if (((is_whites_space(cmd[i + 1]) && ft_check_opened_token(cmd, i+1))
            || (is_r_parenthise(cmd[i + 1]) && !ft_check_quote(cmd, i + 1)) || !cmd[i + 1]
            || (is_tokens(cmd[i + 1]) && !ft_check_quote(cmd, i + 1) && is_closed_parenthise(cmd, i + 1))) && start != -1)
        {
            ft_add_token(&tokens, cmd, start, i);
            start = -1;
        }
    } 
    return (tokens);
}
    