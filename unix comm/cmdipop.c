#include<stdio.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#define BUFFLEN 50
  void getArg(char *buff,char **arg)
   {
      while(*buff!='\0')
               {
                  while(*buff=='\n'||*buff=='\t'||*buff==' ')
                       *buff++ ='\0';
                  *arg++=buff;
         while(*buff!=' '&&*buff!='\t'&&*buff!='\0'&&*buff!='\n')
                    buff++;
               }
              *arg='\0';
   }
int main()
{
  char buff[BUFFLEN];
  pid_t pid; int status;
  printf("$");  int n;
   while(gets(buff)!=NULL)
    {
       if(buff[strlen(buff)-1]=='\n')
        buff[strlen(buff)-1]='\0';
   
        if((pid=fork())<0)
        printf("fork error \n");
       else  if(pid==0)
         {
              char *arg[64];
                getArg(buff,arg);           
           execvp(arg[0],arg);
         }
        if(pid=waitpid(pid,&status,0)<0)
            printf("error waiting\n");

         printf("\n$");         
    }
}
