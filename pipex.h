/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:06:35 by antofern          #+#    #+#             */
/*   Updated: 2024/09/11 21:53:28 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

