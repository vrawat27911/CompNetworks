#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>

struct msg{
int port;
char c[50];
char select;
};

int main()
{
  	int cfd;
	if((cfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	perror("socket error\n");
	struct sockaddr_in s1;
	s1.sin_family=AF_INET;
	s1.sin_port=htons(1600);
	if(inet_pton(AF_INET,"127.0.0.1",(void*)&s1.sin_addr.s_addr)<1)
	perror("inet error\n");
	if(bind(cfd,(struct sockaddr*)&s1,sizeof(s1))==-1)
	perror("binding error\n");
	if(listen(cfd,10)==-1)
	perror("listning error\n");
	
	int nfd;
	printf("waiting for reqst\n");
	if((nfd=accept(cfd,NULL,NULL))==-1)
	perror("accept error\n");
	printf("reqst accepted\n");
	struct msg m;
	recv(nfd,(void*)&m,sizeof(m),0);
	
	int fd;
	if((fd=socket(AF_INET,SOCK_STREAM,0))==-1)
	perror("socket error\n");
	struct sockaddr_in s2;
	s2.sin_family=AF_INET;
	s2.sin_port=htons(m.port);
	if(inet_pton(AF_INET,m.c,(void*)&s2.sin_addr.s_addr)<1)
	error("inet error\n");
	if(connect(fd,(struct sockaddr*)&s2, sizeof(s2))==-1)
	perror("connection error\n");
	printf("connected..\n");
	
		int pid=fork();

		if(pid==0){
		while(1){
		char buf[100];
		scanf("%s",buf);
		send(fd,(void*)&buf,sizeof(buf),0);
		}
		}
		else
		{
		while(1){
		char buf[100];
		recv(fd,(void*)&buf,sizeof(buf),0);
		printf("%s\n",buf);
		}
		}
	
	return 0;
}
		
			
