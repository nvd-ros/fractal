/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window_event.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnovodra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 20:00:00 by rnovodra          #+#    #+#             */
/*   Updated: 2022/06/01 20:00:00 by rnovodra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void       move(t_fractal *f, double min_x, double min_y,
                double max_x, double max_y)
{
    double c;

    c = (f->max.x - f->min.x) * ZOOM_C;
    f->min.x += min_x * c;
    f->max.x += min_y * c;
    f->min.y += max_x * c;
    f->max.y += max_y * c;
}

void    window_event(t_main *w, SDL_Event *e)
{
    if (e->window.event == SDL_WINDOWEVENT_SHOWN)
        w->flags.draw = 1;
    else if (e->window.event == SDL_WINDOWEVENT_HIDDEN)
        w->flags.draw = 0;
    else if (e->window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
        w->flags.win_focus = 0;
    else if (e->window.event == SDL_WINDOWEVENT_FOCUS_LOST)
        w->flags.win_focus = 0;
    else if (e->window.event == SDL_WINDOWEVENT_ENTER)
        w->flags.mouse_focus = 1;
    else if (e->window.event == SDL_WINDOWEVENT_LEAVE)
        w->flags.mouse_focus = 0;
    else if (e->window.event == SDL_WINDOWEVENT_CLOSE)
        w->flags.quit = 1;
    else if (e->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
    {
        if (!(w->screen = SDL_GetWindowSurface(w->win)))
            throw(w, SDL_ERROR_CODE);
    }
}

void    event(t_main *w, t_fractal *f, SDL_Event *e)
{
    if (e->type == SDL_QUIT)
        w->flags.quit = 1;
    else if (e->type == SDL_WINDOWEVENT)
        window_event(w, e);
    else if (e->type == SDL_MOUSEMOTION) // TODO normal move and key function
    {
        int x,y;
        w->flags.mouse_move = 1;
        if (w->flags.mouse_down_left)
            move(f, REVERSE_MOUSE_DIRECTION * SIGN(e->motion.xrel),
                    REVERSE_MOUSE_DIRECTION * SIGN(e->motion.xrel),
                    REVERSE_MOUSE_DIRECTION * SIGN(e->motion.yrel),
                    REVERSE_MOUSE_DIRECTION * SIGN(e->motion.yrel));
        if (w->flags.mouse_down_left == 0 && w->flags.mouse_move)
        {
            SDL_GetMouseState(&x, &y);
            w->mouse.x = map(x, 0, w->screen->w, f->min.x, f->max.x);
            w->mouse.y = map(y, 0, w->screen->h, f->min.y, f->max.y);
        }
    }
    else if (e->type == SDL_KEYDOWN)
    {
        if (e->key.keysym.sym == SDLK_ESCAPE)
            w->flags.quit = 1;
        else if (e->key.keysym.sym == SDLK_SPACE)
            w->flags.draw++;
        else if (e->key.keysym.sym == SDLK_LEFT)
            move(f, -1, -1, 0, 0);
        else if (e->key.keysym.sym == SDLK_RIGHT)
            move(f, 1, 1, 0, 0);
        else if (e->key.keysym.sym == SDLK_UP)
            move(f, 0, 0, -1, -1);
        else if (e->key.keysym.sym == SDLK_DOWN)
            move(f, 0, 0, 1, 1);
        else if (e->key.keysym.sym == SDLK_EQUALS)
            move(f, 1, -1, 1, -1);
        else if (e->key.keysym.sym == SDLK_MINUS)
            move(f, -1, 1, -1, 1);
        else if (e->key.keysym.sym == SDLK_LEFTBRACKET)
        {
            if (w->max_iter - 10 > 0)
                w->max_iter -= 10;
        }
        else if (e->key.keysym.sym == SDLK_RIGHTBRACKET)
        {
            if (w->max_iter + 10 <= 1000)
                w->max_iter += 10;
        }
        else if (e->key.keysym.sym == SDLK_z)
        {
            if (f->pow < MAX_SUM_POWER)
                f->pow += 1;
        }
        else if (e->key.keysym.sym == SDLK_x)
        {
            if (f->pow > 1)
                f->pow -= 1;
        }
    }
    if (e->type == SDL_MOUSEWHEEL && e->wheel.y != 0) // TODO change move logic
    {
         f->min.x = (w->mouse.x + ((f->min.x - w->mouse.x) * (1.0 + f->zoom_factor * SIGN(e->wheel.y * REVERSE_MOUSE_SCROLL))));
         f->max.x = (w->mouse.x + ((f->max.x - w->mouse.x) * (1.0 + f->zoom_factor * SIGN(e->wheel.y * REVERSE_MOUSE_SCROLL))));
         f->min.y = (w->mouse.y + ((f->min.y - w->mouse.y) * (1.0 + f->zoom_factor * SIGN(e->wheel.y * REVERSE_MOUSE_SCROLL))));
         f->max.y = (w->mouse.y + ((f->max.y - w->mouse.y) * (1.0 + f->zoom_factor * SIGN(e->wheel.y * REVERSE_MOUSE_SCROLL))));
    }
    if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        if (e->button.button == SDL_BUTTON_LEFT)
        {
            SDL_SetWindowGrab(w->win, SDL_TRUE);
            if (SDL_SetRelativeMouseMode(SDL_TRUE) != 0)
                throw(w, SDL_ERROR_CODE);
            w->flags.mouse_down_left = 1;
        }
        else if (e->button.button == SDL_BUTTON_RIGHT)
            w->flags.mouse_down_right = 1;
    }
    if (e->type == SDL_MOUSEBUTTONUP)
    {
        if (e->button.button == SDL_BUTTON_LEFT)
        {
            SDL_SetWindowGrab(w->win, SDL_FALSE);
            if (SDL_SetRelativeMouseMode(SDL_FALSE) != 0)
                throw(w, SDL_ERROR_CODE);
            SDL_WarpMouseInWindow(0, w->mouse.x, w->mouse.y);
            w->flags.mouse_down_left = 0;
        }
    }
    if ((e->type == SDL_MOUSEWHEEL && e->wheel.y != 0) ||
        e->type == SDL_MOUSEMOTION ||
        (e->type == SDL_MOUSEMOTION && w->flags.mouse_down_left) ||
        (e->type == SDL_KEYDOWN &&
        (e->key.keysym.sym == SDLK_LEFT ||
        e->key.keysym.sym == SDLK_RIGHT ||
        e->key.keysym.sym == SDLK_UP ||
        e->key.keysym.sym == SDLK_DOWN ||
        e->key.keysym.sym == SDLK_MINUS ||
        e->key.keysym.sym == SDLK_EQUALS ||
        e->key.keysym.sym == SDLK_x ||
        e->key.keysym.sym == SDLK_z ||
        e->key.keysym.sym == SDLK_RIGHTBRACKET ||
        e->key.keysym.sym == SDLK_LEFTBRACKET)))
        w->flags.draw = 1;
}
