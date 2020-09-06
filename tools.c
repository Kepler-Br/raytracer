#include "tools.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int		get_file_size(char *filename)
{
	const int		buff_size = 100;
	unsigned char	buff[buff_size];
	int				total_size;
	int				fd;
	int				ret;

	total_size = 0;
	fd = open(filename, O_RDONLY);
	while((ret = read(fd, buff, buff_size)))
	{
		if (ret == -1)
			return (-1);
		total_size += ret;
	}
	close(fd);
	return (total_size);
}

char	*read_file(char *filename)
{
	const int	file_size = get_file_size(filename);
	char		*text;
	int			fd;

	if (file_size == -1)
		return (NULL);
	text = (char *)malloc(file_size + 1);
	fd = open(filename, O_RDONLY);
	read(fd, text, file_size);
	text[file_size] = '\0';
	close(fd);
	return (text);
}