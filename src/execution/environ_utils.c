#include "../../includes/mshell.h"

static void	swap_envs(t_env *env1, t_env *env2)
{
	char	*tmp_key;
	char	*tmp_value;

	tmp_key = env1->key;
	tmp_value = env1->value;
	env1->key = env2->key;
	env1->value = env2->value;
	env2->key = tmp_key;
	env2->value = tmp_value;
}

t_env	*sort_env(t_env *env)
{
	t_env	*tmp;
	t_env	*tmp2;
	// char	*key;
	// char	*value;

	tmp = env;
	while (tmp)
	{
		tmp2 = tmp->next;
		while (tmp2)
		{
			if (ft_strcmp(tmp->key, tmp2->key) > 0)
				swap_envs(tmp, tmp2);
			tmp2 = tmp2->next;
		}
		tmp = tmp->next;
	}
	return (env);
}

static t_env	*copy_env_node(t_env *node)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (node->key)
		new_node->key = ft_strdup(node->key);
	else
		new_node->key = NULL;
	if (node->value)
		new_node->value = ft_strdup(node->value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

t_env	*copy_env(t_env *env)
{
	t_env	*tmp;
	t_env	*new;
	t_env	*head;

	if (env == NULL)
		return (NULL);
	tmp = env;
	head = copy_env_node(tmp);
	new = head;
	tmp = tmp->next;
	while (tmp)
	{
		new->next = copy_env_node(tmp);
		new = new->next;
		tmp = tmp->next;
	}
	return (head);
}
