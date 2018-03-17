#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#define buffsize 10000
void inout()
{
  char c[buffsize];  int n;
   while((n=read(STDIN_FILENO,c,buffsize))>0)
{
        if(write(STDOUT_FILENO,c,n)!=n)
         { printf("error occurred in writing!!"); exit(1);}
}
  
   if(n<0)
   {printf("error occurred in reading!!"); exit(1);}
}
int main(int n,char *argv[])
{
  if(n>=2)
  {
      n--; int i=1;
    while(n--)
     {
         if(strcmp(argv[i],"-")==0)
          {i++; inout();}
      else
        {
         FILE *fp=fopen(argv[i],"r");
             if(fp==NULL)
                {
                    printf("invalid file %s",argv[i]);
                     exit(1);
                }
              char c=fgetc(fp);
             while(c!=EOF)
              {
                  printf("%c",c);   c=fgetc(fp);
              }
           fclose(fp); i++;
        }
     }
  }
  else if (n==1)
    {
      inout(); 
    }
}
