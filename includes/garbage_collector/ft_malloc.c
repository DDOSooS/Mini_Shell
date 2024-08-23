#include "header.h"

void	ft_lstclear_mshell(t_list **lst)
{
	t_list	*tmp;

	while (lst && *lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		free(tmp->content);
		free(tmp);
	}
}

void	*ft_malloc(size_t size, t_list **list)
{
	t_list	*curr;
	t_list	*tmp;

	curr = *list;
	tmp = (t_list *)malloc(sizeof(t_list));
	if (!tmp)
		return (NULL);
	tmp->content = malloc(size);
	if (!(tmp->content))
	{
		free(tmp);
		return (NULL);
	}
	tmp->next = NULL;
	if (!curr)
		*list = tmp;
	else
	{
		while (curr->next)
			curr = curr->next;
		curr->next = tmp;
	}
	return (tmp->content);
}
