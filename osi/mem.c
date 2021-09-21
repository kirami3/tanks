#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	main(int argc, char *argv[])
{
	void	*ptr;

	while (1)
	{
		for (int i; i < 10; i++)
		{
		ptr = malloc(100*1024*1024);
		memset(ptr, 0xff, 100*1024*1024);
		}
	}
	while (1);
	return (0);
}
