/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnovodra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 20:00:00 by rnovodra          #+#    #+#             */
/*   Updated: 2022/06/01 20:00:00 by rnovodra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
#define FRACTOL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <SDL2/SDL.h>
#include <errno.h>

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

#define BUFF_SIZE 1024
#define STATUS_SUCCESS 0
#define TITLE "Fractol"
#define DEFAULT_HEIGHT 800
#define DEFAULT_WIDTH 800
#define NUM_FRACTALS 1
#define REVERSE_MOUSE_DIRECTION -1
#define REVERSE_MOUSE_SCROLL -1
#define ZOOM_MOUSE_C 1.7
#define ZOOM_C 0.01
#define MAX_ITERATION 500
#define MAX_SUM_POWER 11
#define OPENCL_PROGRAMS {"src/opencl/math.cl", "src/opencl/fractals.cl"}
#define OPENCL_KERNEL_NAME "kernel_fractal"

#define ERRNO_CODE 0
#define SDL_ERROR_CODE 1

#define ABS(x)  (((x) < 0) ? -(x) : (x))
#define SIGN(x)  ((x > 0) - (x < 0))

#define ABS(x)  (((x) < 0) ? -(x) : (x))
#define SIGN(x)  ((x > 0) - (x < 0))

typedef struct      s_flags
{
    unsigned char   quit:1;
    unsigned char   draw:1;
    unsigned char   mouse_focus:1;
    unsigned char   mouse_move:1;
    unsigned char   win_focus:1;
    unsigned char   mouse_down_left:1;
    unsigned char   mouse_down_right:1;
}                   t_flags;

typedef struct      s_cl
{
    cl_platform_id      platform_id;
    cl_uint             num_platforms;
    cl_device_id        device_id;
    cl_uint             num_devices;
    cl_context          context;
    cl_command_queue    command_queue;
    cl_program          program;
    cl_kernel           kernel;
}                   t_cl;

typedef struct      s_color
{
    unsigned char   r;
    unsigned char   g;
    unsigned char   b;
}                   t_color;

typedef struct      s_point
{
    double           x;
    double           y;
}                   t_point;

typedef struct      s_pixel
{
    t_point         p;
    int             iter;
    t_color         color;
}                   t_pixel;

typedef struct      s_fractal
{
    t_point         min;
    t_point         max;
    double           zoom_factor;
    int             (*func)(int, double, double);
    int             pow;
    t_color         color;
}                   t_fractal;

typedef struct      s_main
{
    SDL_Window      *win;
    SDL_Surface     *screen;
    t_flags         flags;
    t_cl            cl;
    t_point         mouse;
    int             max_iter;
}                   t_main;

int     init_opencl(t_main *w);
int     init_sdl(t_main *w);
int     init_fractals(t_main *w, t_fractal **f);
void    menu(t_main *w);

void    loop(t_main *w, t_fractal *f);
void    event(t_main *w, t_fractal *f, SDL_Event *e);
void    draw(t_main *w, t_fractal *f);

//int     mandelbrot(int num_iter, double x, double y);
int     iter_color(int nbr_iter, SDL_PixelFormat *f);
int     set_color(const double i, const int min, const int max, t_color color, SDL_PixelFormat *f);

double    *histogram(t_pixel *p, size_t iter_size, const int max_iter);


double     map(double v, double old_min, double old_max,
                    double new_min, double new_max);

void sum_of_pow_im(double x, double y, double *res_x, double *res_y, int pow);


void    print_clBuildLog(t_main *w);
void    throw(t_main *w, int error_code);
void    print_error(int err);

char    *strjoin(char const *s1, char const *s2);
char    *get_file_data(const char *filename);

#endif //FRACTOL_H
