
#include "../../../includes/mshell.h"
void    ft_free_herdoc(t_herdoc **herdocs)
{
    t_herdoc *tmp;
    t_herdoc *tmp2;
    
    if (!herdocs || !*herdocs)
        return ;

    tmp = *herdocs;
    while (tmp)
    {
        tmp2 = tmp->next;
        if (tmp)
        {
            // free(tmp->delimiter);
            free(tmp);
        }
        tmp = tmp2;
    }
    tmp = NULL;
}

int ft_count_expanded_len(char *delimimiter)
{
    int counter;
    int i;
    char  quote;

    i = -1;
    counter = 0;
    while (delimimiter[++i])
    {
        if (is_quote(delimimiter[i]) && delimimiter[i + 1] && delimimiter[i] == delimimiter[i + 1])
        {
            i++;
            continue;
        }
        if (is_quote(delimimiter[i]))
        {
            quote = delimimiter[i];
            i++;
            while (delimimiter[i] && delimimiter[i] != quote)
            {
                counter++;
                i++;
            }
        }
        else
            counter++;
    }
    return (counter);    
}

void    ft_gen_expanded_delimiter(char **new_del, char *del)
{
    int  i;
    char quote;

    i = -1;
    while (del[++i])
    {
        if (is_quote(del[i]) && del[i + 1] && del[i] == del[i + 1])
        {
            i++;
            continue;
        }
        if (is_quote(del[i]))
        {
            quote = del[i];
            i++;

            while (del[i] && del[i] != quote)
            {
                ft_strcat_char(*new_del, del[i]);
                i++;
            }
        }
        else
            ft_strcat_char(*new_del, del[i]);
    }
}

char *ft_get_delimiter(char *delimiter)
{
    int     len;
    char    *new_delimiter;

    len = ft_count_expanded_len(delimiter);
    new_delimiter = malloc(sizeof(char) * len + 1);
    if (! new_delimiter)
        return (NULL);
    new_delimiter[0]='\0';
    ft_gen_expanded_delimiter(&new_delimiter, delimiter);
    return (new_delimiter);
}

char  *ft_trim_delimiter_quotes(char *delimiter)
{
    char *trimed_delimter;

    trimed_delimter = NULL;
    if (is_exist_quote(delimiter))
        trimed_delimter = ft_get_delimiter(delimiter);
    else
        trimed_delimter = delimiter;
    return (trimed_delimter);
}

t_herdoc *ft_new_herdoc(char *delimiter)
{
    t_herdoc *new;
    new = malloc(sizeof(t_herdoc));
    if (!new)
        return (NULL);
    new->delimiter = delimiter;
    // new->delimiter = ft_expand(delimiter);
    // ft_trim_delimiter_quotes(&new->delimiter);
    new->next = NULL;
    return (new);
}


int ft_add_herdoc(t_herdoc **root, char *del)
{
    t_herdoc    *new;
    t_herdoc    *tmp;

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
        tmp = tmp->next;
    tmp->next = new;
    new->id = tmp->id + 1;
    return (1);
} 

t_herdoc *ft_gen_herdocs(t_token *tokens)
{
    t_herdoc *herdoc;

    herdoc = NULL;
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
    return (herdoc);
}
