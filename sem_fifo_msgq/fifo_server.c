#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/types.h>

char serverbuf[1000];
char clintbuf[1000];
int main()
{
int a,fds,fdc,n;
if((a=mkfifo("/home/gaurav/os/clintserver/servertext.txt",0666))<0)
printf("\n fifo error");
if((a=mkfifo("/home/gaurav/os/clintserver/clinttext.txt",0666))<0)
printf("\n fifo error");
fds=open("/home/gaurav/os/clintserver/servertext.txt",O_RDWR);
fdc=open("/home/gaurav/os/clintserver/clinttext.txt",O_RDWR);
printf("\n welcome to  server process");
while(1)
{

printf("\n server::\t");
gets(clintbuf);
write(fdc,clintbuf,strlen(clintbuf));
printf("\n clint::\t");
n=read(fds,serverbuf,1000);
write(1,serverbuf,n);
}


return 0;
}
