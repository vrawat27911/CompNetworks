#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
char childbuf[200];
char parentbuf[200];

int main()
{int fdp[2],fdc[2],n;
pid_t pid;

if(pipe(fdp)<0)
printf("\n parent pipe error");
if(pipe(fdc)<0)
printf("\n child pipe error");
if((pid=fork())==0)
{
printf("\n welcome to child process");
close(fdp[1]);
close(fdc[0]);
while(1)
{printf("\n message from parent");
n=read(fdp[0],childbuf,200);
write(1,childbuf,n);
printf("\n message to parent");
gets(parentbuf);
write(fdc[1],parentbuf,strlen(parentbuf));
}}
else
{close(fdp[0]);
close(fdc[1]);
printf("\n welcome to parent process");
while(1)
{
printf("\n message to child");
gets(childbuf);
write(fdp[1],childbuf,strlen(childbuf));
printf("\n message from child");
n=read(fdc[0],parentbuf,200);
write(1,parentbuf,n);
}
} 
waitpid(pid,NULL,0);
return 0;
}




