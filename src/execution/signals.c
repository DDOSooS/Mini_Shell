#include "../../includes/mshell.h"

void exit_status(int sig)
{
	g_mshell.exit_value = 128 + sig;
}

void quit_sig(int sig)
{
	write(1, "Quit: 3\n", 8);
	if (sig == SIGQUIT)
		exit_status(sig);
}

void ctl_sig(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		exit_status(sig);
	}
}

void intu_sig(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		exit_status(sig);
	}
}

void herdoc_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_mshell.exit_value = 0;
		exit_status(sig);
	}
}

void prompt_sig(t_signal *sig)
{
	ignore_signals(&sig->sig_int, SIGINT, ctl_sig);
	ignore_signals(&sig->sig_quit, SIGQUIT, SIG_IGN);
}

void exec_signal(t_signal *sig)
{
	ignore_signals(&sig->sig_int, SIGINT, intu_sig);
	ignore_signals(&sig->sig_quit, SIGQUIT, quit_sig);
}

void child_sig(t_signal *sig)
{
	ignore_signals(&sig->sig_int, SIGINT, SIG_DFL);
	ignore_signals(&sig->sig_quit, SIGQUIT, SIG_DFL);
}

void heredoc_sig_p(t_signal *sig)
{
	ignore_signals(&sig->sig_int, SIGINT, herdoc_signal);
}

// void heredoc_sig_c(t_signal *sig)
// {
// 	ignore_signals(&sig->sig_int, SIGINT, SIG_IGN);
// }

void ignore_signals(struct sigaction *signals, int sig, void (*handler)(int))
{
	signals->sa_handler = handler;
	signals->sa_flags = 0;
	sigemptyset(&signals->sa_mask);
	sigaction(sig, signals, NULL);
}
