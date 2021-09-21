#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <time.h>
#include <string.h>
#include <sys/mman.h>

int	main(int argc, char *argv[])
{
	int fd, x, y, bpp;
	char* fb;
	int buffer_size;

	struct fb_var_screeninfo info;
	if ((0) > (fd = open("/dev/fb0", O_RDWR)))
	{
		perror("open");
		return __LINE__;
	}

	if ((-1) == (ioctl(fd, FBIOGET_VSCREENINFO, &info)))
	{
		perror("ioctl");
		goto err1;
	}
	 bpp = (info.bits_per_pixel >> 3);
	buffer_size = info.xres * info.yres * (info.bits_per_pixel >> 3);
	if (MAP_FAILED == (fb = mmap(NULL, buffer_size, PROT_WRITE, MAP_SHARED, fd, 0)))
	{
		perror("mmap");
		goto err1;
	}
	printf("xres = %d\n", info.xres);
	printf("yres = %d\n", info.yres);
	printf("bits_per_pixel = %d\n", info.bits_per_pixel);
for (y = 0; y < info.yres; y++)
{
	usleep(100);
	for (x = 0; x < info.xres; x++)
	{
		memset(fb + (x + y * info.xres) * bpp, 0xff, bpp);
	}
}
munmap(fb, buffer_size);

err1:
	close (fd);
	return 0;
}
