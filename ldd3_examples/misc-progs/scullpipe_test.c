#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <poll.h>
#define OPEN_MAX 256
int main(int argc, char **argv)
{
	int fd = -1;
	int fd_max = 0;
	char dev_name[20]="/dev/scullpipe";
	unsigned int WriteLen = 0;
	unsigned int ReadLen = 0;
	unsigned char CurrentOps[2] = {0};
	unsigned char *Readbuff = NULL;
	unsigned char *Writebuff = NULL;
	int ret = -1;
	unsigned int timeout =0;
	struct pollfd clientfd[OPEN_MAX];
	char ops[3][3]={"r","w","rw"};
	fd = open(dev_name,O_RDWR|O_NONBLOCK);
	if(fd < 0)
	{
		printf("%s open failed return is %d\n",dev_name,fd);
		exit(0);
	}
	fd_max = fd;
	if(argc >= 3)
	{
		//CurrentOps = tolower(argv[1][0]);
		if(strcmp(argv[1],ops[0])==0)
		{
			ReadLen = atoi(argv[2]);
			printf("current ops is read len is %d\n",ReadLen);
		}
		if(strcmp(argv[1],ops[1])==0)
		{
			WriteLen = atoi(argv[2]);
			printf("current ops is write len is %d\n",WriteLen);
		}
		if(argc == 4)
		{
			if(strcmp(argv[1],ops[2])==0)
			{
				ReadLen = atoi(argv[2]);
				WriteLen = atoi(argv[3]);
				printf("current ops is read & write rlen is %d wlen is %d\n",ReadLen,WriteLen);
			}
		}
		
	}
	   //clientfd ³õÊ¼»¯  
    clientfd[0].fd = fd;  
    
	if(ReadLen > 0)
	{
	Readbuff = (char *)malloc(ReadLen);
	if(Readbuff == NULL)
		{
		 printf("get readbuff failed \n");
		 exit(0);
		}
	memset(Readbuff,0,ReadLen);
	 clientfd[0].events = POLLIN;
	}
	if(WriteLen > 0)
	{
	Writebuff = (char *)malloc(WriteLen);
	if(Writebuff == NULL)
		{
		 printf("get Writebuff failed \n");
		 free(Readbuff);
		 exit(0);
		}
	memset(Writebuff,'W',WriteLen);
	 clientfd[0].events |= POLLOUT;
	}
	timeout=3000; 
	while(1)
	{
		ret = poll(clientfd, fd_max+1, -1);
		if(ret < 0)  
		{  
		    printf("select error\n");  
		    break;  
		}  
		else if(ret == 0)  
		{  
		    printf("timeout ...\n");  
		    continue;  
		}
	if (clientfd[0].revents & POLLOUT)//POLLRDNORM  
	{
		if(WriteLen > 0)
		{
			ret = write(fd,Writebuff,WriteLen);
			printf("write ret is %d\n",ret);
		}
	}
	else if (clientfd[0].revents & POLLIN)//POLLRDNORM  
	{
		if(ReadLen > 0)
		{
			ret = read(fd,Readbuff,ReadLen);
			printf("read ret is %d\n",ret);
		}
	}
}
	 
}

