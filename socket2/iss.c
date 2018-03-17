#include<stdio.h>
#include<strings.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<sys/select.h>
#include<sys/time.h>
int main()
{
int fd[2];
struct sockaddr_in b;
char buf[20],buf1[200];
fd[0]=socket(AF_INET,SOCK_STREAM,0);
struct sockaddr_in a1;
a1.sin_family=AF_INET;
a1.sin_port=htonl(6000);
inet_pton(AF_INET,"127.0.0.1",&a1.sin_addr.s_addr);
if(bind(fd[0],(struct sockaddr *)&a1,sizeof(a1))==-1)
printf("error with binding\n");
listen(fd[0],10);
fd[1]=socket(AF_INET,SOCK_STREAM,0);
struct sockaddr_in a2;
a2.sin_family=AF_INET;
a2.sin_port=htonl(6050);
inet_pton(AF_INET,"127.0.0.1",&a2.sin_addr.s_addr);
if(bind(fd[1],(struct sockaddr *)&a2,sizeof(a2))==-1)
printf("error with binding\n");
listen(fd[1],10);
fd_set rset;FD_ZERO(&rset);
FD_SET(fd[0],&rset);
FD_SET(fd[1],&rset);
int i=0,max;
if(fd[0]>fd[1])
max=fd[0];
else
max=fd[1];
int maxfdp1=max+1;
//printf("yes\n");
if((max=select(maxfdp1,&rset,NULL,NULL,NULL))==-1)
printf("error with selection\n");
else if(max>0)
printf("%d\n",max);
while(i<2)
{
if(FD_ISSET(fd[i],&rset))
break;
i++;
}
printf("%d\n",i);
int len=sizeof(b);
int nsfd=accept(fd[i],(struct sockaddr *)&b,&len);
if(nsfd==-1)
printf("error with connection\n");
else
printf("connected\n");
int x=fork();
if(x==0)
{
for(max=0;max<2;max++)
{
if(max!=i)
close(fd[max]);
}
dup2(nsfd,0);
dup2(nsfd,1);
switch(i)
{
case 0:
execlp("/home/sanjay/socket/lab/iss/s1","s1",NULL);
case 1:
execlp("/home/sanjay/socket/lab/iss/s2","s2",NULL);
}
}
else if(x>0)
{
wait();
close(nsfd);
}
}
