# include "../../../includes/mshell.h"

void	ft_parse_cmd(t_tnode **root, t_token **tokens)
{
	t_tnode *new;

	new = ft_new_tnode(0, *tokens);
	if (!*root)
		*root = new;
	else
	{
		new->t_parent = *root;
		if 	(!(*root)->t_left)
			(*root)->t_left = new;
		else
			(*root)->t_right = new;
	}
}

void ft_parse_parenthise(t_tnode **root, t_token **tokens)
{
    t_token *tmp;
    t_tnode *new;
    t_token *tParenthise;
    t_token *tmp1;

    tmp = *tokens;
    while (tmp && tmp->value && !is_l_parenthise(tmp->value[0]))
        	tmp = tmp->next;
	free(tmp->value);
	free(tmp->next->next->value);
	tmp->value = NULL;
	tmp->next->next->value = NULL;
	new = ft_new_tnode(4, *tokens);
	ft_insert_node(root, &new);
	tParenthise = ft_tokinizer(tmp->next->value);
	tmp1 = tParenthise;
	while (tmp1->next)
		tmp1 = tmp1->next;
	tmp1->next = tmp->next->next;
	free(tmp->next->value);
	free(tmp->next);
	tmp->next = tParenthise;
	ft_parse_ast(&new, &tParenthise);
}
