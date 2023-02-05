/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnovodra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 20:00:00 by rnovodra          #+#    #+#             */
/*   Updated: 2022/06/01 20:00:00 by rnovodra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int main(void)
{
    t_main *w;
    t_fractal *f;

    if (!(w = (t_main*)calloc(1, sizeof(t_main))))
        throw(w, ERRNO_CODE);
    errno = 0;
    w->max_iter = MAX_ITERATION;
    init_opencl(w);
    init_sdl(w);
    if (init_fractals(w, &f))
        throw(w, ERRNO_CODE);
    loop(w, f);
    return (STATUS_SUCCESS);
}
