/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractals.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnovodra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 20:00:00 by rnovodra          #+#    #+#             */
/*   Updated: 2022/06/01 20:00:00 by rnovodra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void mandelbrot(const int max_iter, const int pow, const double x0, const double y0, int id, int *res)
{
    double x, y, new_x=0, new_y=0;

    x = x0;
    y = y0;
    for (int i = 0; i < max_iter; i++)
    {
        sum_of_pow_im(x, y, &new_x, &new_y, pow);
        x = new_x + x0;
        y = new_y + y0;
        if (x + y > 4.0)
        {
            res[id] = i;
            return ;
        }
    }
    res[id] = 0;
}

