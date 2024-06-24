#include "../../includes/mshell.h"

t_env *find_env(t_env *env, char *key)
{
	t_env *tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}
int find_env_rem(t_env *env, char *key)
{
	t_env *prev;
    t_env *current;

	prev = NULL;
	current = env;
    while (current != NULL)
	{
        if (ft_strcmp(current->key, key) == 0)
		{
            if (prev == NULL) 
                env = current->next;
            else
                prev->next = current->next; 
            free(current->key);
            free(current->value);
            free(current);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    return 0;
}