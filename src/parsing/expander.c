/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 01:15:21 by aghergho          #+#    #+#             */
/*   Updated: 2024/07/14 00:43:57 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/mshell.h"


# include "../../includes/mshell.h"


int is_delimter(char c, char *delimiter)
{
    int i;

    i = -1;
    while (delimiter[++i])
        if (delimiter[i] == c)
            return (1);
    return (0);
}

int ft_count_word(char *words, char *delimiter)
{
    int i;
    int counter;
    
    i = 0;
    counter = 0;
    while (words[i])
    {
        while (words[i] && is_delimter(words[i], delimiter))
            i++;
        if (words[i])
            counter++;
        while (words[i] && !is_delimter(words[i], delimiter))
            i++;
    }
    return (counter);
}

char **ft_split_word(char **args, char *words, char *delimiter)
{
    int i;
    int j;
    int k;
    
    i = 0;
    k = 0;
    while (words[i])
    {
        j = 0;
        while (words[i] && is_delimter(words[i], delimiter))
            i++;
        if (words[i])
        {
            while (words[i + j] && !is_delimter(words[i + j], delimiter))
				j++;
			args[k++] = ft_substr(words, i, j);
			i += j;
        }     
    }
    args[k]= NULL;
    return (args);
}

char **ft_split_words(char *words, char *delimiter)
{
    int len;
    char **args;

    len = ft_count_word(words, delimiter);
    args = malloc (sizeof(char *) *(len + 1));
    if (!args)
        return (NULL);
    ft_split_word(args, words, delimiter);
    return (args);
}

int ft_count_token_len(char *token)
{
    int i;
    int counter;

    i = -1;
    counter = 0;
    while (token[++i])
    {
        if (is_dollar_sign(token[i])  && is_quote(token[i + 1]) && !is_dollar_sign(token[i - 1]) && !ft_check_quote(token ,i + 1))
            i++;
        else if (is_quote(token[i]) && token[i + 1] && token[i] == token[i + 1] && !ft_check_quote(token, i + 2))
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
    return (pid - 2);
}

void ft_expand_quotes(t_token **tokens)
{
    t_token *tmp;
    char    *tmp_str;
    char    *tmpfile;
    
    tmp = *tokens;
    while (tmp)
    {
        if ((is_exist_quote(tmp->value) || ft_check_dollar(tmp->value)) && tmp->typeId != 7)
        {
            tmpfile = tmp->value;
            tmp_str = ft_expand(tmp->value);
            free(tmp->value);
            tmp->value = tmp_str;
        }
        tmp = tmp->next;
    }
}

int ft_expand_token(t_token **tokens)
{
    t_token *tmp;
    int     flag;

    tmp = *tokens;
    while (tmp)
    {
        flag = 0;
        if (tmp->previous && (tmp->previous->typeId == 7))
            ft_expand_delimiter(&tmp->value);
        else
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

int ft_handle_export_expand(t_token **tokens)
{
    t_token *tmp;
    t_token *next_tmp;
    t_token *last_t_tmp;
    
    tmp = *tokens;
    while (tmp)
    {
        if (tmp->is_exported && ft_check_white_spaces(tmp->value))
        {
            t_token *t_tmp = ft_tokinizer(tmp->value);
            ft_expand_tokens(&t_tmp);
            if (tmp->previous)
                tmp->previous->next = t_tmp;
            else
                *tokens = t_tmp;
            next_tmp = tmp->next;
            free(tmp->value);            
            free(tmp);            
            last_t_tmp = t_tmp;
            while (last_t_tmp->next)
                last_t_tmp = last_t_tmp->next;
            last_t_tmp->next = next_tmp;
            tmp = next_tmp;
        }
        else
            tmp = tmp->next;
    }
    return 1;
}

int ft_check_expand_delimiter(char *delimiter)
{
    if (delimiter && delimiter[0] == '$')
        return (1);
    return (0);
}

t_herdoc *ft_new_herdoc(char *delimiter)
{
    t_herdoc *new;

    new = malloc(sizeof(t_herdoc));
    if (!new)
        return (NULL);
    new->delimiter = delimiter;
    new->is_expanded = ft_check_expand_delimiter(delimiter);
    // new->id = 0;
    new->next = NULL;
    return (new);
}

void    ft_free_herdoc(t_herdoc **herdocs)
{
    t_herdoc *tmp;
    t_herdoc *tmp2;
    
    tmp = *herdocs;
    while (tmp)
    {
        tmp2 = tmp->next;
        free(tmp);
        tmp = tmp2;
    }
    free(tmp);
}

int ft_add_herdoc(t_herdoc **root, char *del)
{
    t_herdoc    *new;
    t_herdoc    *tmp;
    int         herdoc_id;

    herdoc_id = 0;
    new = ft_new_herdoc(del);
    if (!new)
        return (0);
    if (!*root)
    {
        new->id = 0;
        *root = new;
        return (1);
    }
    tmp = *root;
    while (tmp->next)
    {
        // herdoc_id += 1;
        tmp = tmp->next;
    }
    tmp->next = new;
    new->id = tmp->id + 1;
    return (1);
} 

t_herdoc *ft_gen_herdocs(t_token *tokens)
{
    t_herdoc *herdoc;
    t_herdoc *new;

    herdoc = NULL;
    // printf("============(herdooooooooooooooooocs)===============\n");
    while (tokens)
    {
        if (tokens->typeId == 7)
        {
            if (!ft_add_herdoc(&herdoc, tokens->next->value))
            {
                ft_free_herdoc(&herdoc);
                return (NULL);
            }
        }
        tokens = tokens->next;
    }
    // printf("============(herdooooooooooooooooocs)===============\n");
    return (herdoc);
}


int  ft_expand_tokens(t_token **tokens)
{
    t_token *tmp;
    int flag;
    
    ft_expand_quotes(tokens);

    if (!ft_expand_token(tokens))
        return (0);
    ft_handle_export_expand(tokens);
    g_mshell.herdocs = ft_gen_herdocs(*tokens);
    // var_dump_herdocs(g_mshell.herdocs);
    return (1); 
} 