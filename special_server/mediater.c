#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<pthread.h>

struct fds{
int infd;
int outfd;
};

pthread_t tid[10];
int serfd[10];
struct sockaddr_in s[10];

void *serv_fn(void *f)
{
	struct fds fd=*(struct fds*)f;
	int pid=fork();
	if(pid==0)
	{
		while(1){
		char msg[100];
		read(fd.outfd,(void*)&msg,sizeof(msg));
		write(fd.infd,(void*)&msg,sizeof(msg));
		}
	}
	else
	{
		while(1){
		char msg[100];
		read(fd.outfd,(void*)&msg,sizeof(msg));
		write(fd.infd,(void*)&msg,sizeof(msg));
		}
	}
	
}

int main()
{
	int sfd;
	if((sfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	perror("socket error\n");
	struct sockaddr_in s1;
	s1.sin_family=AF_INET;
	s1.sin_port=htons(1500);
	if(inet_pton(AF_INET,"127.0.0.1",(void*)&s1.sin_addr.s_addr)<1)
	perror("inet error\n");
	if(bind(sfd,(struct sockaddr*)&s1,sizeof(s1))==-1)
	perror("binding error\n");
	if(listen(sfd,10)==-1)
	perror("listning error\n");
	
	int i=0;
	while(1)
	{
		int nfd;
		printf("chek 1\n");
		if((nfd=accept(sfd,NULL,NULL))==-1)
		perror("accept error\n"); 
		
		char n;
		read(nfd, (void*)&n,sizeof(n));
		
		
		switch(n) {
		
		case '1':
			{if((serfd[i]=socket(AF_INET,SOCK_STREAM,0))==-1)
			perror("socket error\n");
			
			s[i].sin_family=AF_INET;
			s[i].sin_port=htons(1600);
			if(inet_pton(AF_INET,"127.0.0.1",		(void*)&s[i].sin_addr.s_addr)<1)
			perror("inet error\n");
			if(connect(serfd[i],(struct sockaddr*)&s[i],sizeof(s[i]))==-1)
			perror("connection error\n");
		struct fds f;
		f.infd=nfd;
		f.outfd=serfd[i];
		pthread_create(&tid[i],NULL,&serv_fn,(void*)&f);
		i++;
		
			}
		default:
			printf("error ... \n");
		
		}
	}
return 0;
}
		
		
