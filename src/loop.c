/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnovodra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 20:00:00 by rnovodra          #+#    #+#             */
/*   Updated: 2022/06/01 20:00:00 by rnovodra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void    loop(t_main *w, t_fractal *f)
{
    SDL_Event   e;

    w->flags.draw = 1;
    while (!w->flags.quit) {
        while (SDL_PollEvent(&e))
            event(w, f, &e);
        if (w->flags.draw)
        {
            draw(w, f);
            w->flags.draw = 0;
        }
        w->flags.mouse_move = 0;
    }
}