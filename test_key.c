#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
	int fd,size;
	char key;
	fd = open("/dev/panel", O_RDWR);
	if(fd < 0)
		printf("open failed!\n");
	else
		printf("open success!\n");
	while(1)
	{
		size=read(fd, &key, 1);
		if(size > 0)
			printf("read key:0x%x\n", key);
		//else
		//	printf("read error!\n");
		usleep(5000);
	}
	close(fd);
}
