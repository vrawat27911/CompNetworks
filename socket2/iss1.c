#include<stdio.h>
#include<strings.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<sys/select.h>
#include<sys/time.h>
#include<pthread.h>
void* strt_func1(void * arg)
{
//printf("yes\n");
int *fd=(int *)arg;
int n,n1,n2=0;
recv((*fd),&n,sizeof(int),0);
while(n>0)
{
n1=n%10;
n=n/10;
n2=n2+n1;
}
send((*fd),&n2,sizeof(int),0);
return ((void *)1);
}

void* strt_func2(void * arg)
{
//printf("yes\n");
int *fd=(int *)arg;
char buf[200],buf1[200];
recv((*fd),&buf,sizeof(buf),0);
int i,a=strlen(buf);
for(i=0;i<a;i++)
{
buf1[i]=buf[a-i-1];
}
buf1[i]='\0';
send((*fd),&buf1,strlen(buf1)+1,0);
return ((void *)2);
}

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
pthread_t tid1,tid2;
int x=fork();
if(x==0)
{
//printf("yes\n");
for(max=0;max<2;max++)
{
if(max!=i)
close(fd[max]);
}
switch(i)
{
case 0:
//printf("yes\n");
pthread_create(&tid1,NULL,&strt_func1,&nsfd);
break;
case 1:
//printf("yes\n");
pthread_create(&tid2,NULL,&strt_func2,&nsfd);
break;
}
pthread_join(tid,NULL);
pthread_join(tid2,NULL);
}
else if(x>0)
{
wait();
close(nsfd);
}
}
