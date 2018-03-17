//group1 clients
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/shm.h>
#include<netinet/in.h>
#include<string.h>
#include<pthread.h>

int sfd;
pthread_t p1,p2;
char text[250];

void start_client_send()
{
char msg[200];
char name[100];
printf("enter your name\n");
scanf("%s",name);
printf("%s is now registered for chat\n",name);
while(1)
{
char final[250];
final[0]='\0';
scanf("%s",msg);
strcat(final,name);
strcat(final,":");
strcat(final,msg);
strcpy(text,final);
write(sfd,final,strlen(final)+1);
}
}


//-------------------------------------------------------

void start_client_rcv()
{
char b[250];
while(1)
{
if(read(sfd,b,250)!=0)
{
if(strcmp(text,b)!=0)
printf("%s\n",b);
else
write(sfd,b,strlen(b)+1);
printf("\n");
}
}
}

//------------------------------------------------------------



int main()
{
int fd1=socket(AF_INET,SOCK_STREAM,0);
struct sockaddr_in s;
s.sin_family=AF_INET;
s.sin_port=htons(11000);
inet_pton(AF_INET,"172.30.106.141",(void*)&s.sin_addr.s_addr);
bind(fd1,(struct sockaddr*)&s,sizeof(s));
int fd=socket(AF_INET,SOCK_STREAM,0);
struct sockaddr_in s1;
s1.sin_family=AF_INET;
s1.sin_port=htons(10000);
inet_pton(AF_INET,"172.30.106.141",(void*)&s1.sin_addr.s_addr);
printf("enter choice\n");
int ch;
scanf("%d",&ch);
connect(fd,(struct sockaddr*)&s1,sizeof(s1));
write(fd,(void*)&ch,sizeof(ch));
write(fd,(void*)&s,sizeof(s));
//sleep(3);
close(fd);
printf("port num is %d\n",s.sin_port);
listen(fd1,20);
sfd=accept(fd1,NULL,NULL);
printf("connected to group2 client\n");
pthread_create(&p1,NULL,(void*)&start_client_send,NULL);
pthread_create(&p2,NULL,(void*)&start_client_rcv,NULL);
pthread_join(p1,NULL);
pthread_join(p2,NULL);
close(fd1);
return 0;
}
