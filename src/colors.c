/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnovodra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 20:00:00 by rnovodra          #+#    #+#             */
/*   Updated: 2022/06/01 20:00:00 by rnovodra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

double    *histogram(t_pixel *p, size_t iter_size, const int max_iter)
{
    int *num_iter_per_pix;
    double *histogram_iter;
    int total = 0;

    histogram_iter = (double*)calloc(iter_size, sizeof(double));
    num_iter_per_pix = (int*)calloc((max_iter + 1), sizeof(int));
    if (!num_iter_per_pix && !histogram_iter)
        return (NULL);
    for (size_t i = 0; i < iter_size; ++i)
        num_iter_per_pix[p[i].iter] += 1;
    for (int i = 1; i < max_iter; ++i)
        total += num_iter_per_pix[i];
    for (size_t i = 0; i < iter_size; ++i)
        for (int it = 1; it <= p[i].iter; it++)
            histogram_iter[i] += (double)num_iter_per_pix[it] / (double)total;
    free(num_iter_per_pix);
    return (histogram_iter);
}

int     set_color(const double i, const int min, const int max, t_color color, SDL_PixelFormat *f)
{
    (void)color;
    (void)min;
    (void)max;
    (void)i;

    if (i == 0)
        return 0;
    return (SDL_MapRGB(f, map(i, min, max, 0, 200), 0, 0));
}

