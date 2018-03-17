#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>

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
	char m;
	printf("choose server u want to communicate: ");
	scanf("%c",&m);

	write(cfd,(void*)&m, sizeof(m));
	
	int pid=fork();

		if(pid==0){
		while(1){
		char buf[100];
		recv(cfd,(void*)&buf,sizeof(buf),0);
		printf("%s\n",buf);
		}
		}
		else{
		while(1)
		{
		char buf[100];
		scanf("%s",buf);
		send(cfd,(void*)&buf,sizeof(buf),0);
		}
		}
return 0;
}

	

	
