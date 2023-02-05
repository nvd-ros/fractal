/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnovodra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 20:00:00 by rnovodra          #+#    #+#             */
/*   Updated: 2022/06/01 20:00:00 by rnovodra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int     init_opencl(t_main *m)
{
    cl_int  ret;
    const char *programs[] = OPENCL_PROGRAMS;
    const size_t nbr_programs = sizeof(programs) / sizeof(char *);


    char *p = strjoin("", "");

    for (size_t i = 0; i < nbr_programs; ++i) // TODO write function to read opencl files
    {
        p = strjoin(p, get_file_data(programs[i]));
    }
    if ((ret = clGetPlatformIDs(1, &m->cl.platform_id, &m->cl.num_platforms)) < 0)
        throw(m, ret);
    if (clGetDeviceIDs(m->cl.platform_id, CL_DEVICE_TYPE_GPU, 1,
                       &m->cl.device_id, &m->cl.num_devices) < 0)
        throw(m, ret);
    m->cl.context = clCreateContext(NULL, m->cl.num_devices,
                      &m->cl.device_id, NULL, NULL, &ret);
    if (ret < 0)
        throw(m, ret);
    m->cl.command_queue = clCreateCommandQueueWithProperties(m->cl.context, m->cl.device_id, 0, &ret);
    if (ret < 0)
        throw(m, ret);
    m->cl.program = clCreateProgramWithSource(m->cl.context, 1, (const char **)&p, NULL, &ret);
    if (ret < 0)
        throw(m, ret);
    printf("4\n");
    ret = clBuildProgram(m->cl.program, m->cl.num_devices, &m->cl.device_id, NULL, NULL, NULL);
    printf("5\n");
    if (ret < 0)
    {
        print_clBuildLog(m);
        throw(m, ret);
    }
    printf("2\n");
    m->cl.kernel = clCreateKernel(m->cl.program, OPENCL_KERNEL_NAME, &ret);
    printf("3\n");
    if (ret < 0)
        throw(m, ret);
    return (0);
}

int     init_sdl(t_main *m)
{
    SDL_Init(SDL_INIT_VIDEO);
    if (!(m->win = SDL_CreateWindow(TITLE,
                                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    DEFAULT_WIDTH, DEFAULT_HEIGHT,
                                    SDL_WINDOW_RESIZABLE)))
        throw(m, SDL_ERROR_CODE);
    if (!(m->screen = SDL_GetWindowSurface(m->win)))
        throw(m, SDL_ERROR_CODE);
    return (0);
}

int     init_fractals(t_main *w, t_fractal **f)
{
    if (!(*f = (t_fractal*)calloc(NUM_FRACTALS, sizeof(t_fractal))))
        return (-1);
    f[0]->zoom_factor = 0.2;
    f[0]->min.x = -2.0;
    f[0]->min.y = -1.25;
    f[0]->max.x = 0.5;
    f[0]->max.y = 1.25;
    f[0]->pow = 2;
    f[0]->color.r = 200;
    f[0]->color.g = 0;
    f[0]->color.b = 0;
    (void)w;
    return (0);
}

