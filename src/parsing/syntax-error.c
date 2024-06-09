/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax-error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:51:59 by aghergho          #+#    #+#             */
/*   Updated: 2024/06/09 14:35:46 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/mshell.h"

//world[world]* [|world [world]*]* [[>world][<world][>&world][>>world][>>&world]]* [&& || ()] [&]
//ls -l <<< file should work | cat makefile >| d

int is_logical_operator(char *cmd)
{
    if (cmd[0] && cmd[1] && (is_operator(cmd[0]) || (is_pipe(cmd[0]) && is_pipe(cmd[1]))))
        return (1);
    return (0);
}

int in_redirection(char c)
{
    if (c == '<')
        return (1);
    return (0);
}

int out_redirection(char c)
{
    if (c == '>')
        return (1);
    return (0);
}

int is_pipe(char c)
{
    if (c == '|')
        return (1);
    return (0);
}

int is_operator(char c)
{
    if (c == '&')
        return (1);
    return (0);
}

int is_r_parenthise(char c)
{
    if (c == ')')
        return (1);
    return (0);
}

int is_l_parenthise(char c)
{
    if (c == '(')
        return (1);
    return (0);
}

int is_whites_space(char c)
{
    if (c == ' ' || c == '\t')
        return (1);
    return (0);
}

int is_quote(char c)
{
    if (is_double_quote(c) || is_single_quote(c))
        return (1);
    return (0);
}

int is_single_quote(char c)
{
    if (c == 39)
        return (1);
    return (0);
}

int is_double_quote(char c)
{
    if (c == 34)
        return (1);
    return (0);
}

int is_append(char *cmd)
{
    if (cmd[0] && cmd[1] && out_redirection(cmd[0]) && out_redirection(cmd[1]))
        return (1);
    return (0);
}

int is_herdoc(char *cmd)
{
    if (cmd[0] && cmd[1] && in_redirection(cmd[0]) && in_redirection(cmd[1]))
        return (1);
    return (0);
}

int is_doubled_token(char *cmd)
{
    if (is_herdoc(cmd) || is_append(cmd) || is_logical_operator(cmd))
        return (1);
    return (0);
}

int ft_check_quote(char *cmd_line, int len)
{
	int d_quote;
	int s_quote;
	int i;
	
	i = -1;
	d_quote = 0;
	s_quote = 0;
	while (++i < len)
	{
		if (cmd_line[i] == 34 && !s_quote && !d_quote)
			d_quote = 1;
        else if (cmd_line[i] == 34 && d_quote)
            d_quote = 0;
		if(cmd_line[i] == 39 && !d_quote && ! s_quote)
			s_quote = 1;
        else if (cmd_line[i] == 39 && s_quote)
            s_quote = 0;
	}
	if (d_quote || s_quote)
		return (1);
	return (0);
}

int ft_check_pipe_sequence(char *cmd_line)
{
    int i;
    int len;
    int flag;

    flag = 0;
    i = -1;
    len = ft_strlen(cmd_line);
    while (++i < len)
    {
        if (is_whites_space(cmd_line[i]))
            continue;
        if (cmd_line[i] == '|' && flag)
            return (1);
        if (cmd_line[i] == '|')
            flag++;
        if (cmd_line[i] && cmd_line[i + 1] && cmd_line[i+1])
        {
            flag++;
            i++;
        }
        if (cmd_line[i] != '|')
            flag--;
    }
    return (0);
}

int ft_check_operator_sequence(char *cmd_line)
{
    int i;
    int len;

    i = 0;
    len = ft_strlen(cmd_line);
    while (i < len)
    {
        if (cmd_line[i] == '&' && cmd_line[i + 1] && cmd_line[i + 1] == '&'
            && cmd_line[i + 2] && cmd_line[i + 2] == '&' && !ft_check_quote(cmd_line, i))
            return (1);
		if (cmd_line[i] == '&' && !is_operator(cmd_line[i - 1]) && !is_operator(cmd_line[i + 1]))
		{
            return (1);
        }
        i++;
    }
    return (0);
}

int ft_pipe_sequence_error(char *cmd)
{
    int i;

    i = -1;
    while (cmd[++i])
    {
        if (is_whites_space(cmd[i]))
            continue;
        if (is_pipe(cmd[i]))
        {
            if (cmd[i + 1] && is_pipe(cmd[i + 1]))
                i++;
            while (cmd[++i])
            {
                if (is_whites_space(cmd[i]))
                    continue;
                if (is_pipe(cmd[i]) || is_operator(cmd[i]))
                    return (0);
                else
                    break;
            }
        }
    }
    return (1);
}

int ft_operatore_sequence_error(char *cmd)
{
    int i;
    
    i = -1;
    while (cmd[++i])
    {
        if (is_whites_space(cmd[i]))
            continue;
        if (is_operator(cmd[i]) && !ft_check_quote(cmd, i))
        {
            if (cmd[i + 1] && is_operator(cmd[i + 1]))
                i++;
            else
                return (0);
            while (cmd[++i])
            {
                if (is_whites_space(cmd[i]))
                    continue;
                if (is_pipe(cmd[i]) || is_operator(cmd[i]))
                    return (0);
                else
                    break;
            }
        }   
    }
    return (1);
}

int ft_check_operation_error(char *cmd)
{
	int i;
	int	len;

	i = 0;
	len = ft_strlen(cmd);
	while ( i < len && (cmd[i] == ' ' || cmd[i] == '\t'))
			i++;
    if (cmd[i] && (cmd[i] == '|' || cmd[len-1] == '|'))
        return (ft_putstr_fd("parse error near `|'", 2), 1);
    if (! ft_pipe_sequence_error(cmd))
        return (ft_putstr_fd("parse error near `|'", 2), 1);
    if (cmd[i] && (cmd[i] == '&' || cmd[len-1] == '&'))
        return (ft_putstr_fd("parse error near `&'", 2), 1);
    if (! ft_operatore_sequence_error(cmd))
        return (ft_putstr_fd("parse error near `&'", 2), 1);
	return (0);
}


int ft_check_quote_error(char *cmd_line)
{
	int d_quote;
	int s_quote;
	int i;
	int len;
	
	i = -1;
	len = strlen(cmd_line);
	d_quote = 0;
	s_quote = 0;
	while (++i < len)
	{
		if (cmd_line[i] == 34 && !s_quote && !d_quote)
			d_quote = 1;
        else if (cmd_line[i] == 34 && d_quote)
            d_quote = 0;
		if(cmd_line[i] == 39 && !d_quote && ! s_quote)
			s_quote = 1;
        else if (cmd_line[i] == 39 && s_quote)
            s_quote = 0;
	}
	if (d_quote || s_quote)
		return (ft_printf(";( Syntax error please verify your quotes"),1);
    // ft_printf("===> Quote test pass successfully\n");
	return (0);
}

int ft_check_redirection_sequence(char *cmd_line)
{
    int i;
    int len;

    i = -1;
    len = ft_strlen(cmd_line);
    while (++i < len)
    {
        if (cmd_line[i] == '>' && cmd_line[i + 1] && cmd_line[i + 1] == '>'
            && cmd_line[i + 2] && cmd_line[i + 2] == '>' &&  !ft_check_quote(cmd_line, i))
        {
            perror("parse error near `>'");
            return (1);
        }
        if (cmd_line[i] == '<' && cmd_line[i + 1] && cmd_line[i + 1] == '<'
            && cmd_line[i + 2] && cmd_line[i + 2] == '<' &&  !ft_check_quote(cmd_line, i))
        {
            perror("parse error near `<'");
            return (1);
        }
    }
    return (0);
}

int ft_check_file_name(char *cmd_line)
{
    int i;
    
    i = -1;
    while (cmd_line[++i] && is_whites_space(cmd_line[i]));
    if (!cmd_line[i])
        return (ft_printf("parse error near `\\n'"),0);
    if (is_operator(cmd_line[i]) && cmd_line[i + 1] && is_operator(cmd_line[i + 1]))
        return (ft_printf("parse error near `&&'"), 0);
    if (is_operator(cmd_line[i]))
        return (ft_printf("parse error near `&'"), 0);
    else if (is_pipe(cmd_line[i]) && cmd_line[i + 1] && is_pipe(cmd_line[i + 1]))
        return (ft_printf("parse error near `||'"), 0);
    else if (is_pipe(cmd_line[i]) )
        return (ft_printf("parse error near `|'"), 0);
    else if (in_redirection(cmd_line[i]))
        return (ft_printf("parse error near `<'"), 0);
    else if (out_redirection(cmd_line[i]))
        return (ft_printf("parse error near `>'"), 0);
    else if (is_r_parenthise(cmd_line[i]) || is_r_parenthise(cmd_line[i]))
        return (ft_printf("parse error near `)'"), 0);
    return (1);
}

int ft_check_in_redirection(char *cmd_line , int len)
{
    int i;
    
    i = -1;
    while (++i < len)
    {
        if (in_redirection(cmd_line[i]) && ! ft_check_quote(cmd_line, i))
        {
            if (cmd_line[i + 1] && in_redirection(cmd_line[i + 1]) && !ft_check_quote(cmd_line, i))
            {
                if (!ft_check_file_name(&cmd_line[i + 2]))
                   return (0);
            }
            else
            {
                if (!ft_check_file_name(&cmd_line[i + 1]))
                    return (0);
            }
        }
    }
    return (1);
}

int ft_check_out_redirection(char *cmd_line, int len)
{
    int i;

    i = -1;
    while (++i < len)
    {
        if (out_redirection(cmd_line[i]) && ! ft_check_quote(cmd_line, i))
        {
            if (cmd_line[i + 1] && out_redirection(cmd_line[i + 1]) && !ft_check_quote(cmd_line, i))
            {   
                if (! ft_check_file_name(&cmd_line[i + 2]))
                   return (0);
            }
            else
                if (!ft_check_file_name(&cmd_line[i + 1]))
                    return (0);
        }
    }   
    return (1);
}

int ft_check_redirection_error(char *cmd_line)
{
    int len;

    len = ft_strlen(cmd_line);
    if (ft_check_redirection_sequence(cmd_line))
        return (0);
    // ft_printf("===> redirection sequence test pass successfully\n");
    if (! ft_check_in_redirection(cmd_line, len))
        return ( 0);
    // ft_printf("===> redirection test pass successfully\n");
    if (! ft_check_out_redirection(cmd_line, len))
        return (0);
    return (1);
}

int ft_check_parenthise(char *cmd_line)
{
	int i;

	i = 0;
	while (cmd_line[i] && is_whites_space(cmd_line[i]))
        i++;
    if (cmd_line[i] && is_r_parenthise(cmd_line[i]))
            return (ft_printf("syntax error near unexpected token `()'\n"), 0);      
	if (cmd_line[i] && (!is_operator(cmd_line[i]) || !is_pipe(cmd_line[i]) || !is_r_parenthise(cmd_line[i]) || !is_l_parenthise(cmd_line[i])))
	{
		if (is_l_parenthise(cmd_line[i]))
			return (ft_printf("syntax error near unexpected token `('\n"), 0);
		else
			return (ft_printf("syntax error near unexpected token `%s'\n", &cmd_line[i]), 0);
	}
	return (1);
}


int ft_check_closed_parenthise(char *cmd_line)
{
    int i;
    int len;
    int closed_parenth;

    len = ft_strlen(cmd_line);
    closed_parenth = 0;
    i = -1;
    while (++i < len)
    {
        if (is_r_parenthise(cmd_line[i]) && !ft_check_quote(cmd_line, i) && !closed_parenth)
            return (ft_printf("parse error near `)'"), 0);
        else if (is_r_parenthise(cmd_line[i]) && !ft_check_quote(cmd_line, i))
            closed_parenth--;
        if (is_l_parenthise(cmd_line[i]) && !ft_check_quote(cmd_line, i)  )
            closed_parenth++;
    }
    if (closed_parenth)
    {
        ft_printf("parenthisec are not closed correctly");
        return (0);
    }
    return (1);
}

int is_closed_parenthise(char *cmd_line, int len)
{
    int i;
    int closed_parenth;

    closed_parenth = 0;
    i = -1;
    // ft_printf("cmd_line;(%s) | (%d)\n", cmd_line, len);
    while (++i < len)
    {
        if (is_r_parenthise(cmd_line[i]) && !ft_check_quote(cmd_line, i) && !closed_parenth)
            return (0);
        if (is_r_parenthise(cmd_line[i]) && !ft_check_quote(cmd_line, i))
            closed_parenth--;
        if (is_l_parenthise(cmd_line[i]) && !ft_check_quote(cmd_line, i)  )
            closed_parenth++;
    }
    if (closed_parenth)
        return (0);
    return (1);
}

int ft_check_empty_parenthise(char *cmd_line)
{
    int i;
    int len;

    i = 0;
    len = ft_strlen(cmd_line);
    while (i < len)
    {
        if (is_l_parenthise(cmd_line[i]))
        {
            i++;
            while (cmd_line[i] && is_whites_space(cmd_line[i]))
                i++;
            if (is_r_parenthise(cmd_line[i]) || is_logical_operator(&cmd_line[i]) || is_pipe(cmd_line[i]))
			    return (ft_printf("syntax error near unexpected token `()'\n"), 0);
        }
        i++;
    }
    return (1);
}

int is_word_character(char c)
{
    if (is_pipe(c)|| is_operator(c) || in_redirection(c) || out_redirection(c))
        return (0);
    return (1);
}


int ft_check_left_parenthise(char *cmd_line, int i)
{
    // int flag;

    // flag = 0;
    while (cmd_line[--i] && is_l_parenthise(cmd_line[i]));
	// if (cmd_line[i] && (in_redirection(cmd_line[i]) || out_redirection(cmd_line[i])))
	// 	i--;
	while (cmd_line[i] && !is_pipe(cmd_line[i]) && !is_operator(cmd_line[i]))
    {
        if (is_l_parenthise(cmd_line[i]))
        {
            i--;
            continue;
        }
	    // ft_printf("=====char (%c)====\n", cmd_line[i]);
		if (!is_whites_space(cmd_line[i]) && cmd_line[i])
			return(0);
        i--;
    }
    // if(!flag)
    //     return (0);
	// ft_printf("=====left====\n");
    return (1);
}

int is_redirection(char c)
{
    if (in_redirection(c) || out_redirection(c))
        return (1);
    return (0);
}



int ft_check_right_parenthise(char *cmd)
{
    int i;
    int redirection_found ;

    redirection_found= 0;
    i = -1;
    while (cmd[++i] && is_r_parenthise(cmd[i]));
    while (cmd[i] && !is_pipe(cmd[i]) && !is_operator(cmd[i]))
    {
        if (is_whites_space(cmd[i]) || is_r_parenthise(cmd[i]))
        {
            i++;
            continue;
        }
        if (is_redirection(cmd[i]))
        {
            redirection_found = 1;
            i++;
            continue;
        }
        if (redirection_found)
        {
            while (cmd[i] && !is_whites_space(cmd[i]) && !is_pipe(cmd[i]) && !is_operator(cmd[i]))
                i++;
            redirection_found = 0;
        } 
        else
            return 0;
    }
    return 1;
}

int ft_check_parenthise_sequence(char *cmd_line)
{
    int i;

    i = -1;
    while (cmd_line[++i])
    {
        if (is_l_parenthise(cmd_line[i]) && !ft_check_quote(cmd_line, i))
        {
            if (!ft_check_left_parenthise(cmd_line, i))
                return (0);
        }
        if (is_r_parenthise(cmd_line[i]) && !ft_check_quote(cmd_line, i))
        {
            if (!ft_check_right_parenthise(&cmd_line[i+1]))
                return (0);
        } 
    }
    // ft_printf("====parenthise sequence is done====\n");
    return (1);
}

int ft_check_parenthise_error(char *cmd_line)
{
    if (! ft_check_closed_parenthise(cmd_line) || !ft_check_empty_parenthise(cmd_line) )
        return (0);
    if (! ft_check_parenthise_sequence(cmd_line))
        return (ft_putstr_fd("SDSyntax error near unexpected token `('\n", 2),0);
    return (1);
}

int ft_check_empty_command(char *cmd_line)
{
    int i;
    int len;

    i = 0;
    len = ft_strlen(cmd_line);
    while (i < len && is_whites_space(cmd_line[i]))
        i++;
    if (i == len)
        return (1);
    return (0);
}

int ft_check_syntax(char *cmd_line)
{
    if (!ft_strlen(cmd_line) || ft_check_empty_command(cmd_line))
        return (1);
    if (ft_check_operation_error(cmd_line) || ft_check_quote_error(cmd_line)
        || !ft_check_redirection_error(cmd_line) || !ft_check_parenthise_error(cmd_line))
        exit(EXIT_FAILURE);
    return (1);
}
	
