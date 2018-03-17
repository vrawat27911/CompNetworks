#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<sys/types.h>

int main()
{
  char s[50];
    strcpy(s,"/proc/");
     DIR *dp;
  struct dirent *dirp;
   FILE *fp;
  if((dp=opendir(s))==NULL)
  {printf("error occurred\n"); exit(1);}
  printf("PID STATE PPID PGRP  SESSION  COMM\n");
   while((dirp=readdir(dp))!=NULL)
   {
      strcpy(s,"/proc/");
   char c[6];
   strcpy(c,dirp->d_name);
     if(isdigit(c[0]))
        {
           //pid_t pid=atoi(c);
           strcat(s,c);
       strcat(s,"/stat");
    
           if((fp=fopen(s,"r"))!=NULL)
{
      int pid,ppid,session,pgrp; char state; char comm[20];
fscanf(fp,"%d %s %c %d %d %d",&pid,comm,&state,&ppid,&pgrp,&session);
  printf("%d     %c     %d    %d    %d     %s\n",pid,state,ppid,pgrp,session,comm);
       fclose(fp);
}
        }

  }  closedir(dp);
   exit(0);
}
