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

double     map(double v, double old_min, double old_max,
                            double new_min, double new_max)
{
    return ((v - old_min) / (old_max - old_min) * (new_max - new_min) + new_min);
}

void    draw_fractal(t_main *m, SDL_Surface *s, t_fractal *f, t_pixel *p)
{
    int i;
    double *histogram_iters;

    SDL_LockSurface(s);
    i = 0;
    histogram_iters = histogram(p, m->screen->w * m->screen->h, m->max_iter);
    if (!histogram_iters)
        throw(m, ERRNO_CODE);
    (void)m;
    while (i < s->h * s->w)
    {
        *((Uint32 *) s->pixels + (i % s->w) + (i / s->w) * s->w) = set_color(map(histogram_iters[i], 0, 1, 0, m->max_iter), 0, m->max_iter, f->color, s->format);
        i++;
    }
    free(histogram_iters);
    SDL_UnlockSurface(s);
}

void    set_arr(SDL_Surface *s, t_fractal *f, t_point *p) // TODO move it to OpenCL
{
    int i;

    i = 0;
    while (i < s->w * s->h)
    {
        p[i].x = map(i % s->w, 0, s->w, f->min.x, f->max.x);
        p[i].y = map(i / s->w, 0, s->h, f->min.y, f->max.y);
        ++i;
    }
}

void    draw(t_main *m, t_fractal *f) // TODO Refactor this
{
    cl_int ret;

    cl_mem mem_points = clCreateBuffer(m->cl.context, CL_MEM_READ_ONLY,
                      m->screen->w * m->screen->h * sizeof(t_point), NULL, &ret);
    cl_mem mem_mouse = clCreateBuffer(m->cl.context, CL_MEM_READ_ONLY,
                      sizeof(t_point), NULL, &ret);
    cl_mem mem_max_iter = clCreateBuffer(m->cl.context, CL_MEM_READ_ONLY,
                                     sizeof(int), NULL, &ret);
    cl_mem mem_pow = clCreateBuffer(m->cl.context, CL_MEM_READ_ONLY,
                                    sizeof(int), NULL, &ret);
    cl_mem mem_pixels = clCreateBuffer(m->cl.context, CL_MEM_WRITE_ONLY,
                      m->screen->w * m->screen->h * sizeof(t_pixel), NULL, &ret);
    if (ret < 0)
        throw(m, ret);

    t_point *points = (t_point*)malloc(sizeof(t_point) * m->screen->w * m->screen->h);
    set_arr(m->screen, f, points);

    ret = clEnqueueWriteBuffer(m->cl.command_queue, mem_points, CL_TRUE, 0,
                               m->screen->w * m->screen->h * sizeof(t_point), points, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(m->cl.command_queue, mem_mouse, CL_TRUE, 0,
                               sizeof(t_point), &m->mouse, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(m->cl.command_queue, mem_max_iter, CL_TRUE, 0,
                               sizeof(int), &m->max_iter, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(m->cl.command_queue, mem_pow, CL_TRUE, 0,
                               sizeof(int), &f->pow, 0, NULL, NULL);
    if (ret < 0)
        throw(m, ret);

    ret = clSetKernelArg(m->cl.kernel, 0, sizeof(cl_mem), (void *)&mem_points);
    ret = clSetKernelArg(m->cl.kernel, 1, sizeof(cl_mem), (void *)&mem_mouse);
    ret = clSetKernelArg(m->cl.kernel, 2, sizeof(cl_mem), (void *)&mem_max_iter);
    ret = clSetKernelArg(m->cl.kernel, 3, sizeof(cl_mem), (void *)&mem_pow);
    ret = clSetKernelArg(m->cl.kernel, 4, sizeof(cl_mem), (void *)&mem_pixels);

    if (ret < 0)
        throw(m, ret);
    size_t global_item_size = m->screen->w * m->screen->h;
    ret = clEnqueueNDRangeKernel(m->cl.command_queue, m->cl.kernel, 1, NULL,
                                 &global_item_size, NULL, 0, NULL, NULL);
    if (ret < 0)
        throw(m, ret);

    t_pixel *pixels = (t_pixel*)malloc(sizeof(t_pixel) * m->screen->w * m->screen->h);
    ret = clEnqueueReadBuffer(m->cl.command_queue, mem_pixels, CL_TRUE, 0,
                          m->screen->w * m->screen->h * sizeof(t_pixel), pixels, 0, NULL, NULL);
    if (ret < 0)
        throw(m, ret);

    draw_fractal(m, m->screen, f, pixels);
    if (m->flags.mouse_down_right)
    {
        printf("mouse.x=%f mouse.y=%f\n", m->mouse.x, m->mouse.y);
        printf("iteration at mouse point=%d\n\n", pixels[(int)(m->mouse.x + m->mouse.y * m->screen->w)].iter);
        m->flags.mouse_down_right = 0;
    }
    SDL_UpdateWindowSurface(m->win);
    ret = clReleaseMemObject(mem_points);
    ret = clReleaseMemObject(mem_pixels);
    free(pixels);
    free(points);
}