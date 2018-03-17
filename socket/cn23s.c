#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<netinet/in.h>
#include<pthread.h>
#include<string.h>
#include<poll.h>
struct sockaddr_in info[3];


void connection_special_server(void* arg)
{
int *myfd=(int*)arg;
int choice;
read(*myfd,(void*)&choice,sizeof(choice));
int fd2=socket(AF_INET,SOCK_STREAM,0);
connect(fd2,(struct sockaddr*)&info[choice-1],sizeof(info[choice-1]));
printf("connection successful connected to server %d: fds are %d & %d\n",choice,*myfd,fd2);
struct pollfd fds[2];
fds[0].fd=*myfd;
fds[1].fd=fd2;
fds[0].events=POLLIN;
fds[1].events=POLLIN;
while(1)
{
int x=poll(fds,2,0);
if(fds[0].revents & POLLIN)
{
char buf[200];
read(fds[0].fd,buf,200);
write(fds[1].fd,buf,strlen(buf)+1);
}
else if(fds[1].revents & POLLIN)
{
char buf[200];
read(fds[1].fd,buf,200);
write(fds[0].fd,buf,strlen(buf)+1);
}
}
}





int main()
{
int fd=socket(AF_INET,SOCK_STREAM,0);
int start_num=100;
struct sockaddr_in s1;
s1.sin_family=AF_INET;
s1.sin_port=htons(10000);
inet_pton(AF_INET,"172.30.106.141",(void*)&s1.sin_addr.s_addr);
if(bind(fd,(struct sockaddr*)&s1,sizeof(s1))==-1)
perror("binding error\n");

if(listen(fd,15)==-1)
perror("listening error\n");

int i;
for(i=0;i<3;i++)
{
info[i].sin_family=AF_INET;
info[i].sin_port=htons(12000+i);
inet_pton(AF_INET,"172.30.106.141",(void*)&info[i].sin_addr.s_addr);
}


while(1)
{

int sfd=accept(fd,NULL,NULL);
//dup2(sfd,start_num);
pthread_t p;
pthread_create(&p,NULL,(void*)connection_special_server,(void*)&sfd);

}

close(fd);
return 0;
}

