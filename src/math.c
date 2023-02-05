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

static size_t factorial(size_t n)
{
    size_t  res = 1;

    while (n)
        res *= n--;
    return res;
}

static double fpow(double n, int p)
{
    double res = 1.0;
    while (p--)
        res = res * n;
    return res;
}

static long int binomial(long int n, long int k)
{
    if (n < k)
        return -1;
    return factorial(n) / (factorial(k) * factorial(n - k));
}

void sum_of_pow_im(double x, double y, double *res_x, double *res_y, int pow)
{
    int pow_x = pow, pow_y = 0, sign = 1;
    double res_term;

    while (pow_x >= 0)
    {
        res_term = (double)binomial(pow, pow_y) * fpow(x, pow_x) * fpow(y, pow_y);
        if (pow_y % 4 == 0 || (pow_y - 1) % 4 == 0)
            sign = 1;
        else
            sign = -1;
        if (pow_y % 2 == 0)
            *res_x += res_term * sign;
        else
            *res_y += res_term * sign;
        pow_x--;
        pow_y++;
    }
}
