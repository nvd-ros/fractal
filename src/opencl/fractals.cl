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

__kernel void kernel_fractal(__global const t_point *point,
                            __global const t_point *mouse,
                            __global const int *max_iter,
                            __global const int *pow,
                            __global t_pixel *res)
{
    double x, y, new_x, new_y;
    const int id = get_global_id(0);
    int i;

    x = point[id].x;
    y = point[id].y;
    for (i = 1; i < *max_iter && x*x + y*y <= (1 << 4); i++)
    {
        sum_of_pow_im(x, y, &new_x, &new_y, *pow);
//        new_x = x*x - y*y;
//        new_y = 2*x*y;
        x = new_x + point[id].x;
        y = new_y + point[id].y;
    }
    if (x*x + y*y > 4)
        res[id].iter = i;
    else
        res[id].iter = 0;
    res[id].p.x = x;
    res[id].p.y = y;
}
