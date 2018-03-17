#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>

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
	s1.sin_port=htons(1500);
	if(inet_pton(AF_INET,"127.0.0.1",(void*)&s1.sin_addr.s_addr)<1)
	error("inet error\n");
	
	if(connect(cfd,(struct sockaddr*)&s1, sizeof(s1))==-1)
	perror("connection error\n");
	struct msg m;
	printf("choose server u want to communicate: ");
	scanf("%c",&m.select);
	m.port=1300;

	char som[50]="127.0.0.1";
	int i;
	for(i=0;i<strlen(som);i++)
	m.c[i]=som[i];
	m.c[i]='\0';
	printf("%s\n",m.c);
	send(cfd,(void*)&m,sizeof(m),0);
	printf("reqst sended\n");

	int fd;
	if((fd=socket(AF_INET,SOCK_STREAM,0))==-1)
	perror("sock error\n");
	struct sockaddr_in s2;
	s2.sin_family=AF_INET;
	s2.sin_port=htons(1300);
	if(inet_pton(AF_INET,m.c,(void*)&s2.sin_addr.s_addr)<1)
	perror("inet error\n");
	if(bind(fd,(struct sockaddr*)&s2,sizeof(s2))==-1)
	perror("binding error\n");
	if(listen(fd,10)==-1)
	perror("listning error\n");
	
		int nsfd; 
		printf("chek 1\n");
		if((nsfd=accept(fd,NULL,NULL))==-1)
		perror("accept error\n");
		printf("ackonoledge recved\n");
	
		int pid=fork();

		if(pid==0){
		while(1){
		char buf[100];
		recv(nsfd,(void*)&buf,sizeof(buf),0);
		printf("%s\n",buf);
		}
		}
		else{
		while(1)
		{
		char buf[100];
		scanf("%s",buf);
		send(nsfd,(void*)&buf,sizeof(buf),0);
		}
		}
return 0;
}
