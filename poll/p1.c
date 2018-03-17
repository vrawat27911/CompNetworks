#include<stdio.h>
#include<string.h>
#include<unistd.h>
int main()
{
char s1[100];
printf("this is process 1\n");
FILE* fp;
fp=popen("/home/sanjay/lab3/p2.exe","r");
int fd=fileno(fp);
dup2(fd,0);
scanf("%s",s1);
printf("%s\n",s1);
pclose(fp);
}
