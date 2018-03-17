#include<stdio.h>
#include<string.h>
int main()
{
printf("this is process 2\n");
char s[200];
read(0,s,512);
printf("%s\n",s);
}
