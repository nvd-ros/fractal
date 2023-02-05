#define ABS(x)  (((x) < 0) ? -(x) : (x))

static size_t factorial(size_t n)
{
    size_t  res = 1;

    while (n)
        res *= n--;
    return res;
}

static double fpow(double n, int p)
{
    double res = 1.0f;
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

static void sum_of_pow_im(double x, double y, double *res_x, double *res_y, int pow)
{
    int pow_x = pow, pow_y = 0, sign = 1;
    double res_term = 0.0f;

    *res_x = 0;
    *res_y = 0;
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
