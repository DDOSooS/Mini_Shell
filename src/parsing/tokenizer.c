/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:14:32 by aghergho          #+#    #+#             */
/*   Updated: 2024/05/14 18:03:47 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/mshell.h"


char * ft_get_token_type(char *token)
{
    if (is_operator(token[0]))
        return ("T_OPERATOR");
    else if (is_pipe(token[0]))
        return ("T_PIPE");
    else if (is_l_parenthise(token[0]))
        return ("T_L_PARENTHISE");
    else if (is_r_parenthise(token[0]))
        return ("T_R_PARENTHISE");
    else if (in_redirection(token[0]))
        return ("T_IN_REDIRECTION");
    else if (out_redirection(token[0]))
        return ("T_OUT_REDIRECTION");
    return ("T_WORD");       
}

int ft_add_token(t_token **tokens, char *cmd_line, int start, int end)
{
    t_token *tmp;
    char    *token;
    t_token *new;
    
    // ft_printf("===passed_her add token==\n");

    if (start == end)
        token =ft_substr(cmd_line, start, 1);
    else    
        token =ft_substr(cmd_line, start, end - start +1);    
    if (! token)
        return (0);
    new = malloc(sizeof(t_token));
    if (!new)
        return (0);
    new->value = token;
    new->type = ft_get_token_type(token);
    new->next = NULL;
    ft_printf("===TOKEN value:%s==\n", new->value);
    ft_printf("===TOKEN Type:%s==\n===========================\n", new->type);
    tmp = *tokens;
    if(!tmp)
    {
        tmp = new;
        return (1);
    }
    
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
    return (1);
}

t_token *ft_tokinizer(char *cmd_line)
{
    t_token *tokens;
    int     i;
    int     start;

    i = -1;
    tokens = NULL;
    start = -1;
    while (cmd_line[++i])
    {
        if (is_whites_space(cmd_line[i]) && !ft_check_quote(cmd_line, i))
            continue;
        else if (is_charachter(cmd_line[i]))
            ft_add_token(&tokens, cmd_line, i , i);
        else if (is_quote(cmd_line[i]) && !ft_check_quote(cmd_line, i))
            start = i;
        else if (is_quote(cmd_line[i]) && ft_check_quote(cmd_line, i))
        {
            if (!ft_add_token(&tokens, cmd_line, start, i))
                return (NULL);
            start = -1;
        }
        else if (!ft_check_quote(cmd_line, i) && start == -1)
            start = i;
        else if ((!cmd_line[i + 1] || is_whites_space(cmd_line[i + 1])) )
        {
            // ft_printf("==%d==%d==\n", start, i);
            if (!ft_add_token(&tokens, cmd_line, start, i))
                return (NULL);
            start = -1;
        }
    }
    return (tokens);
}
