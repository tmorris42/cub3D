#include <unistd.h>
#include <fcntl.h>

#include "libft/libft.h"

int	 		ft_get_chr_index(char c, char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		++i;
	}
	return (-1);
}

static void	ft_free_array(char **array)
{
	int		i;

	i = 0;
	while (array && array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static int	ft_config_other(char *line)
{
	return (0);
}

static int	ft_config_r(char *line)
{
	char	**arr;

	arr = ft_split(line, ' ');
	if (!arr)
		return (-1); //error msg split failed / error parsing map
	if (ft_strncmp(arr[0], "R", 2) || !arr[1] || !arr[2] || arr[3])
	{
		ft_free_array(arr);
		return (-1); //error, invalid R configuration
	}
	ft_printf("\tR: %d, %d\n", ft_atoi(arr[1]), ft_atoi(arr[2]));
	// in reality, don't print, save to configuration struc, and verify that HEIGHT and WIDTH are both > 0
	ft_free_array(arr);
	return (1);
}

static int	ft_config_f(char *line)
{
	return (0);
}

static int	ft_config_c(char *line)
{
	return (0);
}

static int	ft_config_n(char *line)
{
	char	**arr;

	arr = ft_split(line, ' ');
	if (!arr)
		return (-1); //error msg split failed / error parsing map
	if (ft_strncmp(arr[0], "NO", 3) || !arr[1] || arr[2])
	{
		ft_free_array(arr);
		return (-1); //error, invalid NO configuration
	}
	ft_printf("\tNO: %s\n", arr[1]);
	// in reality, don't print, save to configuration struc, verify filename if necc.
	ft_free_array(arr);
	return (1);
}

static int	ft_config_e(char *line)
{
	char	**arr;

	arr = ft_split(line, ' ');
	if (!arr)
		return (-1); //error msg split failed / error parsing map
	if (ft_strncmp(arr[0], "EA", 3) || !arr[1] || arr[2])
	{
		ft_free_array(arr);
		return (-1); //error, invalid NO configuration
	}
	ft_printf("\tEA: %s\n", arr[1]);
	// in reality, don't print, save to configuration struc, verify filename if necc.
	ft_free_array(arr);
	return (1);
}

static int	ft_config_w(char *line)
{
	char	**arr;

	arr = ft_split(line, ' ');
	if (!arr)
		return (-1); //error msg split failed / error parsing map
	if (ft_strncmp(arr[0], "WE", 3) || !arr[1] || arr[2])
	{
		ft_free_array(arr);
		return (-1); //error, invalid NO configuration
	}
	ft_printf("\tWE: %s\n", arr[1]);
	// in reality, don't print, save to configuration struc, verify filename if necc.
	ft_free_array(arr);
	return (1);
}

static int	ft_config_so(char *line)
{
	char	**arr;

	arr = ft_split(line, ' ');
	if (!arr)
		return (-1); //error msg split failed / error parsing map
	if (ft_strncmp(arr[0], "SO", 3) || !arr[1] || arr[2])
	{
		ft_free_array(arr);
		return (-1); //error, invalid NO configuration
	}
	ft_printf("\tSO: %s\n", arr[1]);
	// in reality, don't print, save to configuration struc, verify filename if necc.
	ft_free_array(arr);
	return (1);
}

static int	ft_config_s(char *line)
{
	char	**arr;
	
	if (line[0] == 'S' && line[1] == 'O')
		return (ft_config_so(line));
	arr = ft_split(line, ' ');
	if (!arr)
		return (-1); //error msg split failed / error parsing map
	if (ft_strncmp(arr[0], "S", 2) || !arr[1] || arr[2])
	{
		ft_free_array(arr);
		return (-1); //error, invalid NO configuration
	}
	ft_printf("\tS: %s\n", arr[1]);
	// in reality, don't print, save to configuration struc, verify filename if necc.
	ft_free_array(arr);
	return (1);
}

static int	ft_parse_line(char *line)
{
	int		i;
	int		(*config[8]) (char *map_line);

	config[0] = ft_config_other;
	config[1] = ft_config_r;
	config[2] = ft_config_f;
	config[3] = ft_config_c;
	config[4] = ft_config_n;
	config[5] = ft_config_e;
	config[6] = ft_config_w;
	config[7] = ft_config_s;

	i = 0;
	if (!line || line[0] == '\0')
		return (0);
	// check to make sure you don't seg fault by assuming len of string
	i = ft_get_chr_index(line[0], "RFCNEWS");
	i = (*config[i + 1]) (line);
	if (i == -1)
	{
		ft_printf("PARSING ERROR\n");
	}
	// if i == 0, assume that it was an empty line. 0 means nothing illegal, but not useful
	// if i == 1, something was found to be legal and useful
	//
	// in config_other, first check if it is an empty line, which is legal if not in map
	// then check if it is a map line, which is legal if in map
	// otherwise, return -1
	//
	// when parsing map lines, save each line into a linked lst
	// 		set a variable in the struct (map_width?) to be the maximum foudn value as you go
	// 		once true, max map_width is known, we can convert the linked list into a 2d array
	// 		using malloc, height = number of nodes, width = max_width
	//
	//
	
	return (i);
}

int			ft_parse_file(char *filename)
{
	int		fd;
	int		status;
	char	*line;

	status = 0;
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (-1);
	while (status >= 0)
	{
		status = get_next_line(fd, &line);
		if (ft_parse_line(line) <= 0)
			ft_printf("%s\n", line);
		free(line);
		line = NULL;
		if (status == 0)	
			break ;
	}
	return (0);
}

int		main(void)
{
	ft_parse_file("example.cub");
	return (0);
}
