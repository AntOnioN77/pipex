/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:12:18 by antofern          #+#    #+#             */
/*   Updated: 2024/09/11 22:01:13 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

void run_cmd(char *arg, envp ,t_fds io)
{
	char **cmd;

	cmd = ft_split(arg);
	
}

//el struct io contiene como io.in el fd del archivo de entrada
//y en io.out el fd del archivo de salida, el modo (APPEND o REPLACE)
//se gestiono prebiamente al llamar a open()
void secuence_cmds(int amount, t_args args, t_fds io)
{
	int	i;

	i = 1;
	while(i < args.c)
	{
		run_cmd(args.v[i], io);
	}
}

int	main(void)
{

}