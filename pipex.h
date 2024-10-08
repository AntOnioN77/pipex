/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:06:35 by antofern          #+#    #+#             */
/*   Updated: 2024/10/04 14:03:41 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#define READ_END 0 
#define WRITE_END 1 

typedef int fd; 
typedef struct s_fds
{
	fd	in;
	fd	out;
}	t_fds;

typedef struct s_args
{
	int		c;
	char	**v;
	char	**envp;
}	t_args;

typedef int t_pipe[2];

typedef struct s_pipe_set
{
	t_pipe *pipes;
	int		amount;
	int		current;
}	t_pipe_set;
