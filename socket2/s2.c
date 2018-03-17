#include<stdio.h>
#include<string.h>
int main()
{
char buf[200],buf1[200];
recv(0,&buf,sizeof(buf),0);
int i,a=strlen(buf);
for(i=0;i<a;i++)
{
buf1[i]=buf[a-i-1];
}
buf1[i]='\0';
send(1,&buf1,strlen(buf1)+1,0);
}
