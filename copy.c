#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>


int main(int argc,char *argv[])
{
	char *copyby = argv[1];
	char *copyto = argv[2];
	char *size = argv[3];
	char *begin = argv[4];

	int fwd1,fwd2;
	int len = atoi(size);
	int pbegin = atoi(begin);

	char buffer[4096];
	memset(buffer,0,sizeof(buffer));

	fwd1 = open(copyby,O_RDWR);
	fwd2 = open(copyto,O_RDWR|O_CREAT,0664);
	
	lseek(fwd1,pbegin,SEEK_SET);
	if((len = read(fwd1,buffer,len)) >= 0)
	{
		lseek(fwd2,pbegin,SEEK_SET);
		write(fwd2,buffer,len);
	}

	return 0;
}
