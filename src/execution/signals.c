#include "../../includes/mshell.h"

void cntl_c(int signo)
{
	if (signo == SIGINT)
	{
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void ignore_signals()
{
	signal(SIGINT, cntl_c);
	signal(SIGQUIT, SIG_IGN);
}