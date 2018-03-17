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
char buf[20],buf1[200];struct sockaddr_in a;
printf("which service do you want :\n");
int num;
scanf("%d",&num);
int fd;
a.sin_family=AF_INET;
inet_pton(AF_INET,"127.0.0.1",&a.sin_addr.s_addr);
switch(num)
{
case 1:
//printf("yes\n");
fd=socket(AF_INET,SOCK_STREAM,0);
a.sin_port=htonl(6000);
if((connect(fd,(struct sockaddr *)&a,sizeof(a)))==-1)
printf("error with connection\n");
else
printf("connected\n");
printf("enter the number :\n");
int n;
scanf("%d",&n);
send(fd,&n,sizeof(int),0);
recv(fd,&n,sizeof(int),0);
printf("the sum of digits is : %d\n",n);
break;
case 2:
//printf("yes\n");
fd=socket(AF_INET,SOCK_STREAM,0);
a.sin_port=htonl(6050);
if((connect(fd,(struct sockaddr *)&a,sizeof(a)))==-1)
printf("error with connection\n");
else
printf("connected\n");
printf("enter anything :\n");
scanf("%s",buf1);
send(fd,&buf1,strlen(buf1)+1,0);
recv(fd,&buf1,sizeof(buf1),0);
printf("the reverse is : %s\n",buf1);
break;
}
}
