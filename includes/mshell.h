/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshell.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 16:01:09 by aghergho          #+#    #+#             */
/*   Updated: 2024/08/28 11:28:45 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSHELL_H
# define MSHELL_H

# include "./gnl/get_next_line.h"
# include "./libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define BLUE "\001\033[0;34m\002"
# define GREEN "\001\033[0;32m\002"
# define CYAN "\001\033[0;36m\002"
# define RESET "\001\033[0m\002"

typedef struct token
{
	char			*type;
	char			*value;
	int				type_id;
	int				is_exported;
	struct token	*next;
	struct token	*previous;
}					t_token;

typedef struct env
{
	char			*key;
	char			*value;
	int				is_exported;
	int				lock;
	struct env		*next;
}					t_env;

typedef struct history
{
	char			*cmd;
	int				id;
	int				herdoc;
	struct history	*next;
}					t_history;

typedef struct cmd
{
	char			*arg;
	int				check_wildcard;
	int				is_exporterd;
	struct cmd		*next;
}					t_cmd;

typedef struct infile
{
	char			*filename;
	int				is_herdoc;
	int				mode;
	struct infile	*next;
}					t_infile;

typedef struct outfile
{
	char			*filename;
	int				mode;
	struct outfile	*next;
}					t_outfile;

typedef struct redirection
{
	t_infile		*in_file;
	t_outfile		*out_file;
}					t_redirection;

typedef struct treeNode
{
	int				node_type;
	t_cmd			*cmd;
	t_redirection	*redirection;
	struct treeNode	*t_left;
	struct treeNode	*t_parent;
	struct treeNode	*t_right;
}					t_tnode;

enum				e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_OR,
	TOKEN_AND,
	TOKEN_PARENTHISE,
	TOKEN_R_PARENTHISE,
	TOKEN_APPEND,
	TOKEN_HERDOC,
	TOKEN_IN_REDIR,
	TOKEN_OUT_REDIR,
};

enum				e_buitins
{
	EXIT,
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	HISTORY,
};

typedef struct herdoc
{
	char			*delimiter;
	char			*filename;
	int				id;
	int				is_expanded;
	struct herdoc	*next;
}					t_herdoc;

typedef struct mshell
{
	int				exit_value;
	int				n_herdoc;
	int				n_herdoc_executed;
	pid_t			pid;
	t_env			*env;
	t_history		*history;
	t_herdoc		*herdocs;
	t_tnode			*cmd_tree;
	t_token			*token;
}					t_mshell;

extern t_mshell		g_mshell;
int					is_last_right_parentise(char *cmd, int index);
void				varDumpOutFile(t_outfile *redirection);
void				varDumpInFile(t_infile *redirection);
// void	var_dump_token(t_token *tokens);
/*========== lexer functions ============*/

int					is_tokens(char c);
int					is_parenthise(char c);
int					is_or_operator(char *token);
int					is_dollar_sign(char c);
int					is_redirection(char c);
int					is_exist_quote(char *token);
int					in_redirection(char c);
int					is_whites_space(char c);
int					is_double_quote(char c);
int					is_single_quote(char c);
int					is_quote(char c);
int					is_append(char *cmd);
int					is_logical_operator(char *cmd);
int					ft_check_white_spaces(char *cmd);
int					is_exist_quote(char *token);
int					is_herdoc(char *cmd);
int					is_doubled_token(char *cmd);
int					out_redirection(char c);
int					is_pipe(char c);
int					is_operator(char c);
int					is_r_parenthise(char c);
int					is_l_parenthise(char c);
int					is_word_character(char c);
int					is_symbol(char c);
int					is_question_symbol(char c);
int					is_white_space(char c);

/*======== syntax error functions ========*/

int					check_white_spaces(char *cmd);
int					ft_check_syntax(char *cmd_line);
int					ft_check_operation_error(char *cmd);
int					ft_pipe_sequence_error(char *cmd);
int					ft_operatore_sequence_error(char *cmd);
int					ft_check_quote(char *cmd_line, int len);
int					ft_check_quote_error(char *cmd_line);
int					ft_check_left_parenthise(char *cmd_line, int i);
void				ft_check_redirection(char c, int *redirection_found);
int					ft_count_file_name(char *cmd, int index);
int					ft_check_empty_parenthise(char *cmd_line);
int					ft_check_out_redirection(char *cmd_line, int len);
int					ft_check_redirection_error(char *cmd_line);
int					ft_check_parenthise(char *cmd_line);
int					ft_check_closed_parenthise(char *cmd_line);
int					is_closed_parenthise(char *cmd_line, int len);
int					ft_check_redirection_sequence(char *cmd_line);
int					ft_check_in_redirection(char *cmd_line, int len);
int					ft_check_file_name(char *cmd_line);

/*==============tokeinzer function =================*/

t_token				*ft_tokinizer(char *cmd);
int					ft_add_token(t_token **tokens, char *cmd_line, int start,
						int end);
int					ft_get_token_id(char *token);
int					ft_check_opened_token(char *cmd, int len);
int					ft_check_parenthisis_spaces(char *cmd, int index);
int					ft_check_l_parenthise(char *cmd, int i);
int					ft_check_r_parenthise(char *cmd, int i);
int					ft_check_end_token(char *cmd, int i, int start);
int					ft_check_double_token(char *cmd, int i);
int					ft_check_sing_token(char *cmd, int i);
int					ft_check_comments(char *cmd_line);
void				ft_handle_comment(char **cmd_line);
int					is_last_right_parentise(char *cmd, int index);
int					is_last_right_parentise(char *cmd, int index);
int					check_left_p_token(char *token, int index);
int					check_r_parenthis(char *token, int i);
int					check_right_p_token(char *token, int index);
int					check_parenthise_token(char *token, int index);
int					check_parenthis_token(char *cmd, int i, int start);

/*========== expnder functions =====================*/

int					ft_expand_tokens(t_token **tokens, int flag);
int					ft_expand_token(t_token **tokens, int helper);
char				**ft_split_words(char *words, char *delimiter);
int					ft_check_expand_delimiter(char *delimiter);
int					ft_check_expand(char *tmp);
int					ft_check_dollar(char *token);
int					check_closed_quote_sequence(char *token, int index);
int					check_dollar_sign_quoted(char *token, int index);
int					ft_expand_arg(char **arg, int type);
int					ft_gen_expanded_arg(char **str, char *token, int flag);
int					ft_gen_search_expanded_token(char **s1, char *token);
int					ft_gen_pid_token(char **str, char token);
char				*ft_get_env_var(char *str);
void				ft_join_expanded_token(char **joined_str, char *tmp,
						char *str);
int					ft_get_expanded_quoted_token(char *token, int *counter,
						int flag);
int					ft_count_number_len(char token);
int					ft_check_env_var(char *str);
int					check_unclosed_quote(char *token);
pid_t				get_pid(void);
int					ft_expand_herdoc_arg(char **arg);
void				ft_gen_expanded_delimiter(char **new_del, char *del);
void				ft_delete_token(t_token **token, t_token **root);
void				ft_expand_token_helper(int *flag, t_token **token,
						int type);
int					ft_check_unquoted_dollar(char *token);
int					is_expanded_token(char *token, int i);
int					is_pid_token(char *token, int i);
int					check_middle_white_space(char *token);
/*============  generate herdoc functions ==========*/

t_herdoc			*ft_gen_herdocs(t_token *tokens);
int					ft_check_sing_dollor(char *token, int i);
void				ft_trim_delimiter_quotes(char **delimiter);
char				*ft_get_delimiter(char *delimiter);
int					ft_count_expanded_len(char *delimimiter);

/*============  parser functions  ===================*/

void				ft_parse_ast(t_tnode **root, t_token **tokens);
void				ft_parse_parenthise(t_tnode **root, t_token **tokens);
void				ft_parse_cmd(t_tnode **root, t_token **tokens);
void				ft_insert_node(t_tnode **root, t_tnode **new);
int					ft_check_operator(t_token *token);
int					ft_check_parenthises(t_token *tokens);
int					is_last_operator(t_token *tokens);
int					ft_check_and_operator(t_token *token);
int					ft_check_or_operator(t_token *token);
int					check_unclosed_quote(char *token);
int					is_parenthise_redirection(t_token *tokens);
int					ft_check_wild_card(char *arg);
int					ft_check_pipe(t_token *token);
t_cmd				*ft_gen_new_cmds(char *arg);
t_cmd				*ft_split_cmd(char *arg);
void				ft_get_redirection(t_redirection **redirection,
						t_token *token, int inflag, int outflag);
int					ft_add_redirection(t_redirection **root, t_token *token,
						int inredirection, int out_redirection);
t_tnode				*ft_new_tnode(int n_type, t_token *tokens);
int					ft_add_to_cmd(t_cmd **root, char *token);
int					ft_add_cmd(t_cmd **cmd, char *str);

/*================= bultins =================*/
int					ft_cd(t_cmd *cmd, t_mshell *shell);
int					ft_echo(t_cmd *cmd, t_mshell *shell);
int					ft_env(t_cmd *cmd, t_mshell *shell);
int					ft_pwd(t_cmd *cmd, t_mshell *shell);
int					ft_exit(t_cmd *cmd, t_mshell *shell);
int					ft_export(t_cmd *cmd, t_mshell *shell);
int					ft_unset(t_cmd *cmd, t_mshell *shell);
int					ft_history(t_cmd *cmd, t_mshell *shell);

/*================= builtins utils =================*/
t_env				*extarct_node(char *args);
void				appned_export(t_env *env, t_env *new);
void				replacement_export(t_env *env, t_env *new);
int					builtins_finder(t_cmd *cmd, t_mshell *shell, int type);
int					builtins_checker(t_cmd *cmd);
void				put_tohistory(char *cmd, t_history *history, int herdoc);

/*================= execution ===============*/
void				execute(t_tnode *root, t_mshell *shell);
void				ft_execute_tree(t_tnode *root, t_mshell *shell);
void				exec_and_or(t_tnode *root, t_mshell *shell);
void				ft_execute_parenthises(t_tnode *root, t_mshell *shell);
void				run_pipe(t_tnode *root, t_mshell *shell);
int					ft_heredoc(t_tnode *root, t_mshell *shell);
void				ft_execute_cmd(t_tnode *root, t_mshell *shell);
void				cmd_runner(t_cmd *cmd, t_mshell *shell);
void				handle_word(t_tnode *root, t_mshell *shell);
int					get_status(int status);
void				set_under_score(t_env *env, t_cmd *cmd);
int					open_file(char *filename);

/*=============== execution utils ===================*/
char				*check_command(char *cmd, char **paths, int *status);
char				**cmd_args_getter(t_cmd *cmd);
char				**get_path(char *path);
char				**get_envp(t_env *env);
int					check_cmd(char *cmd);

/*================= redirections ==================*/
int					apply_redirections(t_tnode *root);

/*================= ENV ==================*/
char				**get_envp(t_env *env);
void				extarct_env(char **envp, t_env **env);
void				edit_env(t_env *env, char *key, char *value);
void				env_add_back(t_env **env, t_env *new);
void				add_env(t_env *env, char *key, char *value);
int					count_args(t_cmd *cmd);
int					find_env_rem(t_env **env, char *key);
t_env				*find_env(t_env *env, char *key);
t_env				*sort_env(t_env *env);
t_env				*copy_env(t_env *env);
t_env				*find_env(t_env *env, char *key);
t_env				*create_env_node(char *key, char *value, int is_exported);
int					check_tty(void);
int					get_status(int status);
void				extarct_env(char **envp, t_env **env);
t_env				*create_env(void);
void				update_shlvl(t_env *env);

/*================ Clear Allocted ============*/
void				free_env(t_env *env);
void				free_history(t_history *history);
void				free_gvar(int flag);
void				free_func(char **strings);
/*================= Herdoc, red and pipes utils ====================*/
int					heredoc_cheker(char *str, char *filename, int fd);
char				*create_heredoc_filename(int here_doc_num);
void				update_history_from_pipe(int fd, t_history *history);
int					write_to_fd(int fd, char *str);
void				create_heredoc(t_herdoc **herdoc, int write_fd);
void				unlink_herdoc(t_herdoc *herdoc);
/*================= Printers =================*/
void				print_stderr(char *str);
int					export_erorr(char *arg, int status);
void				print_file_error(char *name, char *error);

/*================ wildcards ===================*/
// void				star_expansion(t_cmd *cmd, char ***args);
void				astrict_exp(t_cmd **cmd);
char				**star_expansion(t_cmd *cmd);
int					find_char(char *str, char c);
void				get_list_files(char *exp, char ***file_list);
int					open_dir(char *directory, DIR **dir);
int					check_valid(struct dirent *file_entry, int dir_flag,
						char *exp);
int					check_exp(char *file_name, char *exp);
void				fill_list(char *directory, char *exp, char **file_list,
						int dir_flag);
char				**list_join(char **list_one, char **list_two, int i);
char				*consume_extra(char *reg, char c);
int					match_symbol(char *regexp, char *text);

/*================ var dumping data==============*/
void				ft_free_herdoc(t_herdoc **herdocs);
void				var_dump_herdoc(t_herdoc *herdocs);

/*=============== signals =======================*/
void				handle_signals(void (*sigint)(int), void (*sigquit)(int),
						void (*sigint_old)(int), void (*sigquit_old)(int));
void				active_sigquit(int sig);
void				active_sigint(int sig);
void				interactive_sigquit(int sig);
void				interactive_sigint(int sig);

/*============= expansion ======================*/
// void star_expansion(char ***cmd_args);
// void	star_expansion(char ***cmd_args, int i);
// void star_expansion(t_cmd *cmd);
// void star_expansion(char **cmd_args);
// void	var_dump_token(t_token *tokens);

/*=============== cleaning ressources =============*/
void				ft_free_tokens(t_token **tokens);
void				ft_free_tree(t_tnode **tree);
void				ft_free_redirections(t_redirection *redirection);
void				ft_free_out_files(t_outfile *outfile);
void				ft_free_cmds(t_cmd *cmds);
void				ft_free_out_files(t_outfile *outfile);
void				ft_free_in_file(t_infile *infile);
void				ft_free_cmd_var(void);
int					check_parenthise_token(char *token, int index);
#endif
