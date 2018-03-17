//bypass server
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


int main()
{

struct sockaddr_in info[3];
int i;
for(i=0;i<3;i++)
{
info[i].sin_family=AF_INET;
info[i].sin_port=htons(12000+i);
inet_pton(AF_INET,"172.30.106.141",(void*)&info[i].sin_addr.s_addr);
}

int fd=socket(AF_INET,SOCK_STREAM,0);
struct sockaddr_in s1;
s1.sin_family=AF_INET;
s1.sin_port=htons(10000);
inet_pton(AF_INET,"172.30.106.141",(void*)&s1.sin_addr.s_addr);
if(bind(fd,(struct sockaddr*)&s1,sizeof(s1))==-1)
perror("binding error\n");

if(listen(fd,25)==-1)
perror("listening error\n");

while(1)
{
struct sockaddr_in a;
int p;
int sfd=accept(fd,NULL,NULL);
int choice;
read(sfd,(void*)&choice,sizeof(int));
read(sfd,(void*)&a,sizeof(a));
printf("port num is %d\n",a.sin_port);
close(sfd);
printf("message read from group1:choice is %d\n",choice);
int fd1=socket(AF_INET,SOCK_STREAM,0);
connect(fd1,(struct sockaddr*)&info[choice-1],sizeof(info[choice]));
write(fd1,(void*)&a,sizeof(a));
sleep(3);
close(fd1);
}

close(fd);
return 0;
}

