/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 01:15:21 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/21 18:20:54 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../../includes/mshell.h"

int ft_expand_token(t_token **tokens)
{
    t_token *tmp;
    int     flag;

    tmp = *tokens;
    while (tmp)
    {
        flag = 0;
        if (!tmp->previous || (tmp->previous->typeId != 7))
        {
            if (tmp->value && is_dollar_sign(tmp->value[0]))
            {
                flag = 1;
                tmp->is_exported = 1;
            }
            ft_expand_arg(&tmp->value);
        }
        if (!tmp->value[0] && flag && tmp->previous && (tmp->previous->typeId == 6 || tmp->previous->typeId == 8 || tmp->previous->typeId == 9 ))
            return (ft_putstr_fd("ambiguous redirect\n", 2), 0);
        tmp = tmp->next;
    }
    return (1);
}

void ft_expand_exported_tokens(t_token **tokens, t_token **curr_token)
{
    t_token *t_tmp;
    t_token *next_tmp;
    t_token *last_t_tmp;

    t_tmp = ft_tokinizer((*curr_token)->value);
    ft_expand_tokens(&t_tmp);
    if ((*curr_token)->previous)
        (*curr_token)->previous->next = t_tmp;
    else
        *tokens = t_tmp;
    next_tmp = (*curr_token)->next;
    free((*curr_token)->value);            
    free((*curr_token));           
    last_t_tmp = t_tmp;
    while (last_t_tmp->next)
        last_t_tmp = last_t_tmp->next;
    last_t_tmp->next = next_tmp;
    (*curr_token) = next_tmp;
}

void ft_handle_export_expand(t_token **tokens)
{
    t_token *tmp;

    tmp = *tokens;
    while (tmp)
    {
        if (tmp->is_exported && ft_check_white_spaces(tmp->value))
            ft_expand_exported_tokens(tokens, &tmp);
        else
            tmp = tmp->next;
    }
}


void ft_expand_parenthisis(t_token **tokens)
{
    t_token *temp_token;
    t_token *temp;

    temp_token = *tokens;
    while (temp_token)
    {
        if ((temp_token->typeId == 4  ||  temp_token->typeId == 5  ) && (temp_token->next && temp_token->next->typeId == temp_token->typeId))
        {
            temp = temp_token->next;
            if (temp_token->previous)
                temp_token->previous->next = temp;
            temp->previous = temp_token->previous;
            if (temp_token == *tokens)
                *tokens = temp;
            free(temp_token->value);
            free(temp_token);
            temp_token = temp; 
        }   
        else
            temp_token = temp_token->next;
    }
}

int  ft_expand_tokens(t_token **tokens)
{    
    // ft_expand_quotes(tokens);
    ft_expand_parenthisis(tokens);
    if (!ft_expand_token(tokens))
        return (0);
    ft_handle_export_expand(tokens);
    return (1); 
}  



int ft_count_token_len(char *token)
{
    int i;
    int counter;

    i = -1;
    counter = 0;
    while (token && token[++i])
    {
        if (check_dollar_sign_quoted(token , i) || check_closed_quote_sequence(token , i))
            i++;
        else if (is_quote(token[i]))
        {
            while (token[i] && ft_check_quote(token, i+1))
            {
                i++;
                counter++;
            }
            counter++;
        }
        else if (token[i])
            counter++;
    }
    return (counter);
}

char *ft_gen_token_toexpand(char *str, char *token)
{
    int i;
    int j;

    j = 0;
    i = -1;
    while (token[++i])
    {
        if (is_dollar_sign(token[i]) && token[i + 1] && is_quote(token[i + 1]) && !is_dollar_sign(token[i - 1]) && !ft_check_quote(token, i + 1))
            continue;
        else if (is_quote(token[i]) && token[i + 1] && token[i] == token[i + 1] && !ft_check_quote(token, i + 2))
            i++;
        else if (is_quote(token[i]))
        {
            while (token[i] && ft_check_quote(token, i+1))
                str[j++] = token[i++];
            str[j++] =token[i];
        }
        else if (token[i])
            str[j++] = token[i];
    }
    str[j] = '\0';
    return (str);
}


char    *ft_expand(char *token)
{
    char *str;
    int len;
  
    len = ft_count_token_len(token);
    str = malloc(sizeof(char) * (len + 1));
    if (!str)
        return (NULL);
    str = ft_gen_token_toexpand(str, token);
    return (str);
}

// void ft_expand_quotes(t_token **tokens)
// {
//     t_token *tmp;
//     char    *tmp_str;
    
//     tmp = *tokens;
//     while (tmp)
//     {
//         if ((is_exist_quote(tmp->value) || ft_check_dollar(tmp->value)) && tmp->typeId != 7)
//         {
//             tmp_str = ft_expand(tmp->value);
//             free(tmp->value);
//             tmp->value = tmp_str;
//         }
//         tmp = tmp->next;
//     }
// }