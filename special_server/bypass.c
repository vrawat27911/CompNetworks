#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>

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
	while(1)
	{
		int nfd;
		printf("chek 1\n");
		if((nfd=accept(sfd,NULL,NULL))==-1)
		perror("accept error\n"); 
		printf("chek 2\n");
		struct msg m; int fd;struct sockaddr_in s2;
		recv(nfd,(void*)&m,sizeof(m),0);
		
		switch(m.select) {
		case '1':
			
			{if((fd=socket(AF_INET,SOCK_STREAM,0))==-1)
			perror("socket error\n");
			
			s2.sin_family=AF_INET;
			s2.sin_port=htons(1600);
			if(inet_pton(AF_INET,"127.0.0.1",		(void*)&s2.sin_addr.s_addr)<1)
			perror("inet error\n");
			printf("bypassing reqst\n");
			if(connect(fd,(struct sockaddr*)&s2,sizeof(s2))==-1)
			perror("connection error\n");
			printf("reqst sended\n");
			send(fd, (void*)&m, sizeof(m),0);
			break; }
		case '2': 
			{
			if((fd=socket(AF_INET,SOCK_STREAM,0))==-1)
			perror("socket error\n");
			
			s2.sin_family=AF_INET;
			s2.sin_port=htons(1700);
			if(inet_pton(AF_INET,"127.0.0.1",		(void*)&s2.sin_addr.s_addr)<1)
			perror("inet error\n");
			if(connect(fd,(struct sockaddr*)&s2,sizeof(s2))==-1)
			perror("connection error\n");
			send(fd, (void*)&m, sizeof(m),0);
			break; }
		default:{
			printf("invalid selection \n");
			break; }
			}
		}
 return 0;
}			
