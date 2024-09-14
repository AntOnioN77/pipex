/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:12:18 by antofern          #+#    #+#             */
/*   Updated: 2024/09/14 15:17:26 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>
#include <libft.h>

//Retorna un array de strings con los directorios de la variable
//de entorno PATH
get_paths(char *envp,  char *cmd)
{
	int i;
	char **paths;

	

/*
	paths = ft_split();
	while(paths[i] != NULL)
	{
		access();
		i++;
	}
*/
}

//recibe un array de dos strings donde [0] es la clave
//y [1] su valor. Retorna el valor
char	get_value(char *key_value)

int run_cmd(t_args *args, int narg, t_fds io)
{
	char	**cmd_and_flags;
	
	cmd_and_flags = ft_split(args->v, " ");
	if (cmd_and_flags == NULL)
		return(ERROR);
}

//el struct io contiene como io.in el fd del archivo de entrada
//y en io.out el fd del archivo de salida, el modo (APPEND o REPLACE)
//se gestiono prebiamente al llamar a open()
void secuence_cmds(int amount, t_args *args, t_fds io)
{
	int		i;
	char	*paths;

	paths = get_paths(args->envp,  cmd_and_flags[0]);
	i = 1;
	while(i < args.c)
	{
		run_cmd(args, io);
	}
}

int	main(void)
{

}