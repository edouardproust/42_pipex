/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eproust <contact@edouardproust.dev>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 13:17:33 by eproust           #+#    #+#             */
/*   Updated: 2025/01/19 20:22:46 by eproust          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	execute_command(char *cmd)
{
	char	**argv;
	char	*envp[1];
	char	*exec;

	if (ft_strlen(cmd) == 0)
		ft_error("Error: command argument is empty");
	argv = ft_split(cmd, ' ');
	if (!argv)
		ft_perror("Error executing command (ft_split)");
	exec = ft_strjoin("/bin/", argv[0]);
	if (!exec)
	{
		ft_free_split(&argv);
		ft_perror("Error executing command (ft_join)");
	}
	envp[0] = NULL;
	execve(exec, argv, envp);
	free(argv);
	ft_perror("Error executing command");
}

static void	handle_child_process(int *pipefd, char *infile, char *cmd)
{
	int	infd;

	close(pipefd[0]);
	infd = open(infile, O_RDONLY);
	if (infd == -1)
		ft_perror("Error opening input file");
	if (dup2(infd, STDIN_FILENO) == -1)
		ft_perror("Error duplicating file descr. (input file -> stdin)");
	close(infd);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		ft_perror("Error duplicating file descr. (pipe's write end -> stdout");
	close(pipefd[1]);
	execute_command(cmd);
}

static void	handle_parent_process(int *pipefd, int childpid, char *outfile,
	char *cmd)
{
	int	outfd;
	int	status;

	close(pipefd[1]);
	if (waitpid(childpid, &status, 0) == -1)
		ft_perror("Error waiting for child process");
	outfd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfd == -1)
		ft_perror("Error opening output file");
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		ft_perror("Error duplicating file descr. (pipe's read end -> stdin)");
	close(pipefd[0]);
	if (dup2(outfd, STDOUT_FILENO) == -1)
		ft_perror("Error duplicating file descr. (output file -> stdout)");
	close(outfd);
	execute_command(cmd);
}

int	main(int ac, char **av)
{
	pid_t	pid;
	int		pipefd[2];

	if (ac != 5)
		ft_error("Usage: ./pipex <input_file> <cm1> <cmd2> <output_file>");
	if (access(av[1], R_OK) == -1)
		ft_perror("Error accessing input file");
	if (pipe(pipefd) == -1)
		ft_perror("Error creating pipe");
	pid = fork();
	if (pid == -1)
		ft_perror("Error forking");
	if (pid == 0)
		handle_child_process(pipefd, av[1], av[2]);
	else
		handle_parent_process(pipefd, pid, av[4], av[3]);
	return (EXIT_SUCCESS);
}
