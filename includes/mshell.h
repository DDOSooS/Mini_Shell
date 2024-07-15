/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshell.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:01:09 by aghergho          #+#    #+#             */
/*   Updated: 2024/07/14 00:43:47 by aghergho         ###   ########.fr       */
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
#include <string.h>
#include <signal.h>
#include <limits.h>
#include "./libft/libft.h"
#include "./printf/ft_printf.h" 
#include "./gnl/get_next_line.h"
#include <stdio.h>

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

// typedef struct tree
// {
//     t_tnode **root;
// }   Tree;

enum TokenType
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_IN_REDIRECTION,
	TOKEN_OUT_REDIRECTION,
	TOKEN_LOGICAL_OPERATOR,
	TOKEN_L_PARENTHISE,
	TOKEN_R_PARENTHISE
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
	char 			*filename;
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
} t_mshell;

extern t_mshell g_mshell;

typedef struct builtin
{
	char   *name;
	int     (*func)(t_cmd *cmd, t_mshell *shell);
} t_builtins;


/*
	minishell_functions
*/

/*================ Parsing =================*/
pid_t get_pid();
int ft_check_white_spaces(char *cmd);
void ft_expand_delimiter(char **arg);
void	var_dump_cmd(t_cmd *cmds);
int ft_expand_arg(char **arg);
char **ft_split_words(char *words, char *delimiter);
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
// void ft_parse_input(char *cmd);

/*================= execution ===============*/
void ft_execute_tree(t_tnode *root, t_mshell *shell);
void ignore_signals();
// t_env *extarct_env(char **envp);
void extarct_env(char **envp, t_env **env);
void put_tohistory(char *cmd, t_history *history);
int find_env_rem(t_env *env, char *key);
t_env *find_env(t_env *env, char *key);
// int builtins_finder(t_cmd *cmd, t_mshell *shell);
int builtins_finder(t_cmd *cmd, t_mshell *shell, int type);
int builtins_checker(t_cmd *cmd);

/*================= ENV ==================*/
t_env *find_env(t_env *env, char *key);
void edit_env(t_env *env, char *key, char *value);
t_env *create_env_node(char *key, char *value, int is_exported);
void env_add_back(t_env **env, t_env *new);

void add_env(t_env *env, char *key, char *value);
int count_args(t_cmd *cmd);
char **get_envp(t_env *env);

/*================ Clear Allocted ============*/
void free_env(t_env *env);
void free_history(t_history *history);
void free_gvar(void);
void free_func(char **strings);

/*================= Herdoc, red and pipes ====================*/
void run_pipe(t_tnode *root, t_mshell *shell);
// int ft_heredoc(t_tnode *root, t_mshell *shell);
int ft_heredoc(t_tnode *root, t_mshell *shell);
int heredoc_cheker(char*str, char *filename, int fd);

/*================= Printers =================*/
void print_stderr(char *str);

void    ft_free_herdoc(t_herdoc **herdocs);

/*================var dumping data==============*/
void	var_dump_herdocs(t_herdoc *herdoc);
#endif