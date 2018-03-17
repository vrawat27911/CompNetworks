#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<stdlib.h>
int main(int argc,char *arg[])
{
  if(argc>=2)
  {
    if(argc==2)
  {pid_t pid=atoi(arg[1]);
     kill(pid,SIGTERM); printf("\n killed");}
  else
   {
      pid_t pid=atoi(arg[2]); 
      if(strcmp(arg[1],"-1")==0)
     {kill(pid,SIGHUP); printf("\n killed");}
   else if(strcmp(arg[1],"-2")==0)
     {kill(pid,SIGINT); printf("\n killed");}
   else if(strcmp(arg[1],"-3")==0)
     {kill(pid,SIGQUIT); printf("\n killed");}
    else if(strcmp(arg[1],"-9")==0)
     {kill(pid,SIGKILL); printf("\n killed");}
     else if(strcmp(arg[1],"-15")==0)
     {kill(pid,SIGTERM); printf("\n killed");}
   else if(strcmp(arg[1],"-24")==0)
     {kill(pid,SIGSTOP); printf("\n killed");}
   else if(strcmp(arg[1],"-25")==0)
     {kill(pid,SIGTSTP); printf("\n killed");}
  else if(strcmp(arg[1],"-26")==0)
     {kill(pid,SIGCONT); printf("\n killed");}
   }
  }
  
  else
  {printf("\n lesser arguments needs atleast 2");}
}
