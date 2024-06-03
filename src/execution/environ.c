#include "../../includes/mshell.h"

t_env *extarct_env(char **envp)
{
	int i;
	t_env *env;
	t_env *tmp;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		tmp = (t_env *)malloc(sizeof(t_env));
		tmp->key = ft_substr(envp[i], 0, ft_strchr(envp[i], '=') - envp[i]);
		tmp->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		tmp->next = env;
		env = tmp;
		i++;
	}
	return (env);
}