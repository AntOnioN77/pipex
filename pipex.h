/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:06:35 by antofern          #+#    #+#             */
/*   Updated: 2024/09/18 12:03:50 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

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

