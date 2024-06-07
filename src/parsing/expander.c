/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 01:15:21 by aghergho          #+#    #+#             */
/*   Updated: 2024/06/05 21:05:57 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/mshell.h"

int ft_count_token_len(char *token)
{
    int i;
    int counter;

    i = -1;
    counter = 0;
    while (token[++i])
    {
        if (is_quote(token[i]) && token[i + 1] && token[i] == token[i + 1] && !ft_check_quote(token, i + 2))
        {
            i++;
            continue;
        }
        if (is_quote(token[i]))
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

int  is_exist_quote(char *token)
{
    int i;
    
    i =  -1;
    while (token[++i])
        if (is_quote(token[i]))
            return (1);
    return (0);
}

int is_dollar_sign(char c)
{
    if (c == '$')
        return (1);
    return (0);
}

int ft_check_dollar(char *token)
{
    int i;

    i = -1;
    while (token[++i])
    {
        if (token[i] == '$' && token[i +1])
            return (1);      
    }
    return (0);
}

char *ft_gen_token_toexpand(char *str, char *token)
{
    int i;
    int j;

    j = 0;
    i = -1;
    while (token[++i])
    {
        if (is_quote(token[i]) && token[i + 1] && token[i] == token[i + 1] && !ft_check_quote(token, i + 2))
        {
            i++;
            continue;
        }
        if (is_quote(token[i]))
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

int ft_check_expand(char *tmp)
{
    if (is_exist_quote(tmp) || ft_check_dollar(tmp))
        return (1);
    return (0);
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

pid_t get_pid()
{
    pid_t pid;
    pid = fork();
    if (!pid)
        exit(0);
    return (pid -2);
}

void ft_expand_tokens(t_token *tokens)
{
    t_token *tmp;
    char    *tmp_str;
    char    *tmpfile;
    
    tmp = tokens;
    // ft_printf("======g_pid(%d)==getpid(%d)<<<<<<<<<<<<<<<<<\n", pid - 1, getpid() );
    while (tmp)
    {
        // ft_printf("===>>> EXPANDER  ?(%d) ||| ?(%d)<<====\n",is_exist_quote(tmp->value), ft_check_quote(tmp->value, ft_strlen(tmp->value)));
        if (is_exist_quote(tmp->value) || ft_check_dollar(tmp->value))
        {
            // ft_printf("=======  new tok len (%d)   =========\n\n", ft_count_token_len(tmp->value));
            tmpfile = tmp->value;
            tmp_str = ft_expand(tmp->value);
            // ft_printf("=======  new expanded token (%d)(%s)   =========\n\n", ft_strlen(tmp_str), tmp_str);
            free(tmpfile);
            tmp->value = tmp_str;
            // ft_printf("=======  new expanded token (%d)(%s)   =========\n\n", ft_strlen(tmp->value), tmp->value);
        }
        tmp = tmp->next;
    }
}

// t_cmd *ft_expand_cmd(t_cmd **cmds)
// {
//     t_cmd *tmp;

//     tmp = *cmds;
//     while (tmp)
//     {
//         tmp = tmp->next;
//     }
    
//     return (*cmds);
// }