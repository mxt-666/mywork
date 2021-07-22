#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc,char* argv[])
{
	char* copyby = argv[1];
	char* copyto = argv[2];
	char* process_num = argv[3];

	int process = 10;
	if(process_num != NULL)
	{
		process = atoi(process_num);
		if(process > 100||process < 0)
		{
			printf("进程数目输入错误！\n");
			exit(1);
		}
	}
	
	int sizemax,size,fd;
	if((fd = open(copyby,O_RDWR)) == -1)
	{
		printf("复制文件不存在!\n");
		exit(2);
	}

	if((sizemax = lseek(fd,0,SEEK_END)) == -1)
	{
		printf("复制文件为空！\n");
		exit(3);
	}

	if(sizemax%process != 0)
	{
		size = sizemax/process + 1;
	}
	else
	{
		size = sizemax/process;
	}

	int i;
	pid_t pid;
	for(i = 0;i < process;i++)
	{
		pid = fork();

		if(pid == 0)
			break;
	}

	if(pid > 0)
	{
		int num = 0;
		pid_t wpid;
		while((wpid = waitpid(-1,NULL,WNOHANG)) != -1)
		{
			if(wpid > 0)
			{
				num++;
				if(num == process)
				{
					printf("复制成功！\n");
					return 0;
				}
			}
		}
	}
	else if(pid == 0)
	{
		char copy_size[4096];
		char copy_begin[100];
		int begin = i*size;

		sprintf(copy_size,"%d",size);
		sprintf(copy_begin,"%d",begin);
		
		printf("copyby:%s copyto:%s size:%s begin:%s\n",copyby,copyto,copy_size,copy_begin);
		execlp("/home/colin/colin第五阶段/mywork/copy","copy",copyby,copyto,copy_size,copy_begin,NULL);
	}
	else
	{
		printf("进程创建错误!\n");
		return 0;
	}

	return 0;
}
