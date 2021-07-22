#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/wait.h>

int get_file_size(char* copyby)
{
	int filesize = -1;
	struct stat statbuff;
	if(stat(copyby,&statbuff) < 0)
	{
		return filesize;
	}
	else
	{
		filesize = statbuff.st_size;
	}
	return filesize;
}

int main(int argc,char* argv[])
{
	char* copyby = argv[1];
	char* copyto = argv[2];
	char* process_num = argv[3];

	int num = 10;
	if(argv[3]!=0)
	{
		num = atoi(process_num);
	}
	//int sizemax = get_file_size(copyby);
	
	int fd;
	if((fd = open(copyby,O_RDWR)) == -1)
	{
		printf("复制文件无\n");
		return 0;
	}

	int sizemax;
	int size;

	if(num > 100||num <= 0)
	{
		printf("进程数输入错误\n");
		return 0;
	}
	if((sizemax = lseek(fd,0,SEEK_END)) == -1)
	{
		printf("复制文件为空！\n");
		return 0;
	}

	if(sizemax % num == 0)
	{
		size = sizemax/num;
	}
	else
	{
		size = sizemax/num + 1;
	}

	pid_t pid;
	int i;
	for(i = 0;i < num;i++)
	{
		pid = fork();

		if(pid == 0)
			break;
	}

	int begin = i*size;
	char str_begin[4096];
	sprintf(str_begin,"%d",begin);
	char str_size[4096];
	sprintf(str_size,"%d",size);

	if(pid > 0)
	{
		//execlp("copy","copy",copyby,copyto,str_size,str_begin,NULL);

		pid_t wpid;
		int die = 0;
		while((wpid = waitpid(-1,NULL,WNOHANG)) != -1)
		{
			if(wpid > 0)
			{
				die++;

				if(die == num)
				{
					printf("复制结束！\n");
					return 0;
				}
			}
		}
	}
	else if(pid == 0)
	{
		printf("copyby :%s,copyto :%s,str_size :%s,str_begin :%s\n",copyby,copyto,str_size,str_begin);
		execl("/home/colin/colin第五阶段/mywork/copy","copy",copyby,copyto,str_size,str_begin,NULL);
		exit(1);
	}
	else
	{
		printf("process woring....\n");
		return 0;
	}
	return 0;

}
