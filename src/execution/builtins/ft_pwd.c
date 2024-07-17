#include "../../../includes/mshell.h"

static void	print_pwd(void)
{
	char	*cwd;
	char	*buffer;
	int		size;

	size = 1024;
	buffer = ft_calloc(sizeof(char), size);
	cwd = getcwd(buffer, size);
	while (!cwd)
	{
		free(buffer);
		size *= 2;
		buffer = ft_calloc(sizeof(char), size);
		cwd = getcwd(buffer, size);
	}
	printf("%s\n", cwd);
	free(cwd);
}

int	ft_pwd(t_cmd *cmd, t_mshell *shell)
{
	(void)cmd;
	(void)shell;
	print_pwd();
	return (0);
}
