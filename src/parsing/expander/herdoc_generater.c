
#include "../../../includes/mshell.h"
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

t_herdoc *ft_new_herdoc(char *delimiter)
{
    t_herdoc *new;

    new = malloc(sizeof(t_herdoc));
    if (!new)
        return (NULL);
    new->delimiter = delimiter;
    // new->is_expanded = ft_check_expand_delimiter(delimiter);
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