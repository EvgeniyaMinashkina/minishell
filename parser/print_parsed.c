/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_parsed.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 13:42:03 by tkoval            #+#    #+#             */
/*   Updated: 2026/06/18 18:44:30 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_redirs(t_redir *redir)
{
	while (redir)
	{
		printf("    redir: ");
		if (redir->type == REDIR_IN)
			printf("< ");
		else if (redir->type == REDIR_OUT)
			printf("> ");
		else if (redir->type == APPEND)
			printf(">> ");
		else if (redir->type == HEREDOC)
			printf("<< ");
		else
			printf("? ");

		printf("%s\n", redir->filename);
		redir = redir->next;
	}
}

void	print_argv(char **argv)
{
	int	i = 0;

	if (!argv)
		return ;
	while (argv[i])
	{
		printf("    argv[%d]: %s\n", i, argv[i]);
		i++;
	}
}

void	print_cmds(t_cmd *cmd_list)
{
	int	cmd_index = 0;

	// printf("Enter print_cmds\n");
	if (cmd_list == NULL)
		printf("List is empty\n");

	while (cmd_list)
	{
		printf("Command %d:\n", cmd_index);

		// print arguments
		print_argv(cmd_list->argv);

		// print redirections
		print_redirs(cmd_list->redirs);

		printf("\n");

		cmd_list = cmd_list->next;
		cmd_index++;
	}
}
