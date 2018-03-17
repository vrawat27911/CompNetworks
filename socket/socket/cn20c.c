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
if(connect(fd,(struct sockaddr*)&s1,sizeof(s1))==-1)
perror("connection failure\n");
struct add a1;
printf("enter 2 numbers\n");
scanf("%d",&a1.num1);
scanf("%d",&a1.num2);
while(1)
{
write(fd,(void*)&a1,sizeof(a1));
//sleep(1);
int result;
while(read(fd,(void*)&result,sizeof(result))==0);
printf("answer is %d\n",result);
a1.num1++;
a1.num2++;
}
return 0;
}

