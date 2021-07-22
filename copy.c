#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc,char* argv[])
{
	char* copyby = argv[1];
	char* copyto = argv[2];
	char* copy_size = argv[3];
	char* copy_begin = argv[4];

	int size = atoi(copy_size);
	int begin = atoi(copy_begin);
	int fwd1 = open(copyby,O_RDWR);
	int fwd2 = open(copyto,O_RDWR|O_CREAT,0664);

	char buffer[4096];
	memset(buffer,0,sizeof(buffer));

	lseek(fwd1,begin,SEEK_SET);
	size = read(fwd1,buffer,size);

	lseek(fwd2,begin,SEEK_SET);
	write(fwd2,buffer,size);

	return 0;
}
