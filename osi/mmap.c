#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char* argv[])
{
	char* s1, s2, s3;

	int fd1, fd2, fd3;
	off_t file_size;
	fd1 = open("1.txt", O_RDONLY);
	s1 = malloc(32);
	mmap(s1, 10, PROT_READ, MAP_PRIVATE, fd1, 0);
	printf("file_body %s\n", s1);
//	write(1, ptr->mbr, sizeof(ptr->mbr));
//	munmap(ptr, file_size);


	close(fd1);

	return (0);
}

