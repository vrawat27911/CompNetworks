#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
int main()
{
char s1[100];
printf("this is process 1\n");
int fd[2];
pipe(fd);
int x=fork();
if(x>0)
{
sleep(2);
close(fd[1]);
dup2(fd[0],0);
scanf("%s",s1);
printf("%s\n",s1);
wait();
}
else if(x==0)
{
close(fd[0]);
dup2(fd[1],1);
execl("/home/sanjay/lab3/p2.exe",NULL,NULL);
}
}
