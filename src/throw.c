/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   throw.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnovodra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 20:00:00 by rnovodra          #+#    #+#             */
/*   Updated: 2022/06/01 20:00:00 by rnovodra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void    clean_memory(t_main *w)
{
    clReleaseKernel(w->cl.kernel);
    clReleaseCommandQueue(w->cl.command_queue);
    clReleaseContext(w->cl.context);
    SDL_DestroyWindow(w->win);
    SDL_Quit();
    free(w);
}

void    throw(t_main *w, int error_code) // TODO add "char *message" for more info about error. For example no such file and the name of the file
{
    print_error(error_code);
    clean_memory(w);
    exit(error_code | errno);
}