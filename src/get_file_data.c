#include "main.h"

char    *read_data(const int fd)
{
    char	buff[BUFF_SIZE + 1];
    char	*data;
    char	*tmp;
    int		ret;

    if (read(fd, buff, 0) < 0 || !(data = calloc(1, sizeof(char))))
        return (NULL);
    while ((ret = read(fd, buff, BUFF_SIZE)) > 0)
    {
        buff[ret] = 0;
        tmp = data;
        data = strjoin(data, buff);
        free(tmp);
        if (!data)
            return (NULL);
    }
    if (ret < 0)
    {
        free(data);
        return (0);
    }
    return (data);
}

char	*get_file_data(const char *filename)
{
    char	*data;
    int		fd;

	if ((fd = open(filename, O_RDONLY)) < 0)
		return (NULL);
    data = read_data(fd);
    close(fd);
    return (data);
}