/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshell.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:01:09 by aghergho          #+#    #+#             */
/*   Updated: 2024/07/19 23:15:02 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSELL_H
# define MSELL_H

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <sys/stat.h>
#include "./libft/libft.h"
#include "./printf/ft_printf.h" 
#include "./gnl/get_next_line.h"
#include <stdio.h>

// typedef struct s_signal
// {
// 	struct sigaction	sig_int;
// 	struct sigaction	sig_quit;
// }	t_signal;

typedef struct token
{
	char            *type;
	int             typeId;
	char            *value;
	int             is_exported;
	struct token    *next;
	struct token    *previous;
}   t_token;

typedef struct  env
{
	char        *key;
	char        *value;
	int 		is_exported;
	int 	   	lock;
	struct env  *next;
} t_env;

typedef struct history
{
	int             id;
	int				herdoc;
	char            *cmd;
	struct history  *next;
} t_history;

typedef struct cmd
{
	char            *arg;
	int             check_wildcard;
	int             is_exporterd;
	struct cmd      *next;
} t_cmd;

typedef struct infile
{
	int             is_herdoc;
	char            *filename;
	int             mode;
	struct infile   *next;
}   t_infile;

typedef struct outfile
{
	char                *filename;
	int                 mode;
	struct outfile      *next;
}   t_outfile;

typedef struct redirection
{
	t_infile        *in_file;
	t_outfile        *out_file;
} t_redirection;

typedef struct treeNode
{
	int                 node_type;
	t_cmd            	*cmd;
	t_redirection       *redirection;
	struct treeNode     *t_left;
	struct treeNode     *t_parent;
	struct treeNode     *t_right;
}   t_tnode;

enum TokenType
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_OR,
	TOKEN_AND,
	TOKEN_PARENTHISE,
	// TOKEN_R_PARENTHISE,
} ;

enum buitins
{
	EXIT,
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	HISTORY
};
typedef struct herdoc
{
    int             id;
    char            *delimiter;
    int             is_expanded;
    struct  herdoc  *next;
}   t_herdoc;


typedef struct mshell
{
 t_env      *env;
 t_history  *history;
 int        exit_value;
 pid_t      pid;
 int        n_herdoc;
 int        n_herdoc_executed;
 t_herdoc   *herdocs;
//  t_signal   sig;
} t_mshell;

extern t_mshell g_mshell;

void	varDumpOutFile(t_outfile *redirection);
void	varDumpInFile(t_infile *redirection);
/*================ Parsing =================*/
pid_t get_pid();
int ft_check_white_spaces(char *cmd);
void ft_expand_delimiter(char **arg);
void	var_dump_cmd(t_cmd *cmds);

//this function is reponsible for expanding envp var
int     ft_expand_arg(char **arg);

char    **ft_split_words(char *words, char *delimiter);
void	ft_free_tokens(t_token **tokens);
int is_dollar_sign(char c);
int ft_check_expand(char *token);
void	varDumpOutFile(t_outfile *redirection);
int ft_expand_tokens(t_token **tokens);
void	varDumpInFile(t_infile *redirection);
int ftGetTokenId(char *token);
int ft_get_unexpanded_token(char *token, int *counter);
int ft_check_and_operator(t_token *token);
int ft_check_or_operator(t_token *token);
void ft_parse_ast(t_tnode **root, t_token **tokens);
int is_append(char *cmd);
int is_herdoc(char *cmd);
int is_redirection(char c);
int is_character(char c);
t_token *ft_tokinizer(char *cmd);
int is_closed_parenthise(char *cmd_line, int len);
// void ft_parse_ast(Tree **root, t_token *tokens);
int is_doubled_token(char *cmd);
int is_herapp_redirection(char *cmd);
int is_logical_operator(char *cmd);
int is_herdoc(char *cmd);
int is_append(char *cmd);
void	var_dump_token(t_token *tokens);
t_token *ft_tokinizer(char *cmd_line);
int ft_check_syntax(char *cmd_line);
int in_redirection(char c);
int out_redirection(char c);
int is_pipe(char c);
int is_operator(char c);
int is_r_parenthise(char c);
int is_l_parenthise(char c);
int is_whites_space(char c);
int is_charachter(char c);
int ft_check_quote(char *cmd_line, int len);
int is_single_quote(char c);
int is_double_quote(char c);
int is_quote(char c);
int is_quote(char c);
int ft_add_herdoc(t_herdoc **root, char *del);
t_herdoc *ft_gen_herdocs(t_token *tokens);
// void ft_parse_input(char *cmd);

/*================= bultins =================*/
int		ft_cd(t_cmd *cmd, t_mshell *shell);
int		ft_echo(t_cmd *cmd, t_mshell *shell);
int		ft_env(t_cmd *cmd, t_mshell *shell);
int		ft_pwd(t_cmd *cmd, t_mshell *shell);
int		ft_exit(t_cmd *cmd, t_mshell *shell);
int		ft_export(t_cmd *cmd, t_mshell *shell);
int		ft_unset(t_cmd *cmd, t_mshell *shell);
int		ft_history(t_cmd *cmd, t_mshell *shell);

/*================= builtins utils =================*/
t_env	*extarct_node(char *args);
void	appned_export(t_env *env, t_env *new);
void	replacement_export(t_env *env, t_env *new);
int		builtins_finder(t_cmd *cmd, t_mshell *shell, int type);
int		builtins_checker(t_cmd *cmd);
void	put_tohistory(char *cmd, t_history *history, int herdoc);

/*================= execution ===============*/
void	execute(t_tnode *root, t_mshell *shell);
void	cmd_runner(t_cmd *cmd, t_mshell *shell);
void	ft_execute_tree(t_tnode *root, t_mshell *shell);
int		get_status(int status);

/*=============== execution utils ===================*/
char	*check_command(char *cmd, char **paths, int *status);
void	run_curr(char **cmd_args, char **paths, char **envp);
char	**cmd_args_getter(t_cmd *cmd);
char	**get_path(char *path);
char	**get_envp(t_env *env);

/*================= redirections ==================*/
int		apply_redirections(t_tnode *root, t_mshell *shell);

/*================= ENV ==================*/
char	**get_envp(t_env *env);
void	extarct_env(char **envp, t_env **env);
void	edit_env(t_env *env, char *key, char *value);
void	env_add_back(t_env **env, t_env *new);
void	add_env(t_env *env, char *key, char *value);
int		count_args(t_cmd *cmd);
int		find_env_rem(t_env *env, char *key);
t_env	*find_env(t_env *env, char *key);
t_env	*sort_env(t_env *env);
t_env	*copy_env(t_env *env);
t_env	*find_env(t_env *env, char *key);
t_env	*create_env_node(char *key, char *value, int is_exported);

/*================ Clear Allocted ============*/
void	free_env(t_env *env);
void	free_history(t_history *history);
void	free_gvar(void);
void	free_func(char **strings);
/*================= Herdoc, red and pipes ====================*/
void	run_pipe(t_tnode *root, t_mshell *shell);

// int ft_heredoc(t_tnode *root, t_mshell *shell);
int		ft_heredoc(t_tnode *root, t_mshell *shell);
int		heredoc_cheker(char*str, char *filename, int fd);
char	*create_heredoc_filename(int here_doc_num);

/*================= Printers =================*/
void	print_stderr(char *str);
int		export_erorr(char *arg, int status);

/*================var dumping data==============*/
void	ft_free_herdoc(t_herdoc **herdocs);
void	var_dump_herdocs(t_herdoc *herdoc);

/*=============== signals =======================*/
void	handle_signals(void (*sigint)(int), void (*sigquit)(int), void (*sigint_old)(int), void (*sigquit_old)(int));
void	active_sigquit(int sig);
void	active_sigint(int sig);
void	interactive_sigquit(int sig);
void	interactive_sigint(int sig);

/*============= expansion ======================*/
// void star_expansion(char ***cmd_args);
// void	star_expansion(char ***cmd_args, int i);
// void star_expansion(t_cmd *cmd);
void	star_expansion(t_cmd *cmd, char ***args);
int		find_char(char *str, char c);
// void star_expansion(char **cmd_args);

#endif