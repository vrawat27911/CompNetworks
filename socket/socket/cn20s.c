#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
struct add
{
int num1;
int num2;
};

int main()
{
int fd=socket(AF_INET,SOCK_STREAM,0);
struct sockaddr_in s1;
s1.sin_family=AF_INET;
s1.sin_port=htons(10000);
inet_pton(AF_INET,"192.168.229.135",(void*)&s1.sin_addr.s_addr);
if(bind(fd,(struct sockaddr*)&s1,sizeof(s1))==-1)
perror("binding problem\n");


if(listen(fd,10)==-1)
perror("listen call not working\n");

while(1)
{
int sfd;
if((sfd=accept(fd,NULL,NULL))==-1)
perror("accept call error\n");
printf("sfd is %d\n",sfd);
int pid=fork();
if(pid==0)
{
close(fd);
struct add a;
while(1)
{
read(sfd,(void*)&a,sizeof(a));
int ans=a.num1+a.num2;
write(sfd,(void*)&ans,sizeof(ans));
sleep(2);
}
close(sfd);
exit(0);
}
else
{
close(sfd);
}
}
return 0;
}

