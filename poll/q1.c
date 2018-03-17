#include<stdio.h>
#include<string.h>
#include<unistd.h>
int main()
{
char s1[100];
FILE* fp;
fp=popen("/home/sanjay/lab3/q2.exe","w");
int fd=fileno(fp);
dup2(fd,1);
write(1,"this is process 1\n",200);
pclose(fp);
}
