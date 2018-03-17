#include<stdio.h>
int main()
{
int n,n1,n2=0;
recv(0,&n,sizeof(int),0);
while(n>0)
{
n1=n%10;
n=n/10;
n2=n2+n1;
}
send(1,&n2,sizeof(int),0);
}
