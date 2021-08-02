include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>

typedef struct stack{
	char* by;
	char* to;
	int begin;
	int filesize;
	int size;
}Copy;

void* m_copy(void * arg)
{
	Copy* copy = (Copy*)arg;

	int fwd1,fwd2;
	fwd1 = open(copy->by,O_RDWR);
	fwd2 = open(copy->to,O_RDWR);

	int begin = (copy->size)*(copy->begin);

	char* by_addr = (char*)mmap(NULL,copy->filesize,PROT_READ,MAP_SHARED,fwd1,0);
	char* to_addr = (char*)mmap(NULL,copy->filesize,PROT_READ|PROT_WRITE,MAP_SHARED,fwd2,0);
	
	memcpy(to_addr+begin,by_addr+begin,copy->size);

	munmap(by_addr,copy->filesize);
	munmap(to_addr,copy->filesize);
	close(fwd1);
	close(fwd2);
	pthread_exit((void*)1);
}

int main(int argc,char* argv[])
{
	/*if(argc < 2)
	{
		printf("所传参数过少\n");
		exit(1);
	}*/
	

	int pthread_num = 10;
	if(argv[3] != NULL)
	{
		pthread_num = atoi(argv[3]);
		if(pthread_num < 0||pthread_num > 100)
		{
			printf("pthread_num is wroing..\n");
			exit(2);
		}
	}
	Copy* copy = (Copy*)malloc(sizeof(Copy)*pthread_num);

	int fwd1,fwd2;
	if((fwd1 = open(argv[1],O_RDWR)) < 0)
	{
		printf("复制文件打开错误/不存在\n");
		exit(3);
	}
	fwd2 = open(argv[2],O_RDWR|O_CREAT,0664);

	int filesize;
	if((filesize = lseek(fwd1,0,SEEK_END)) == -1)
	{
		printf("复制文件为空！\n");
		exit(4);
	}
	ftruncate(fwd2,filesize);
	
	int size;
	if(filesize%pthread_num != 0)
	{
		size = filesize/pthread_num + 1;
	}
	else
	{
		size = filesize/pthread_num;
	}
	
	pthread_t* tid = (pthread_t*)malloc(sizeof(pthread_t)*pthread_num);
	//pthread_t tid;
	double degree;
	int err;
	for(int i = 0;i < pthread_num;i++)
	{
		copy[i].by = argv[1];
		copy[i].to = argv[2];
		copy[i].filesize = filesize;
		copy[i].size = size;
		copy[i].begin = i;
		pthread_create(&tid[i],NULL,m_copy,(void*)&copy[i]);
		
		/*
		if((err = pthread_join(tid,NULL)) > 0)
		{
			printf("pthread join is wroing:%s\n",strerror(err));
			exit(5);
		}
		if(i == 9)
		{
			degree = 100.00;
		}
		else
		{
			degree = 100/pthread_num*(i+1);
		}
		printf("[=========================][%2f%%]\n",degree);
		*/
		
	}

	for(int i = 0;i < pthread_num;i++)
	{
					
		if((err = pthread_join(tid[i],NULL)) > 0)
		{
			printf("pthread join is wroing:%s\n",strerror(err));
			exit(5);
		}
		if(i == pthread_num-1)
		{
			degree = 100.00;
		}
		else
		{
			degree = 100/pthread_num*(i+1);
		}
		printf("[=========================][%2f%%]\n",degree);
	}

	close(fwd1);
	close(fwd2);
	return 0;
}
