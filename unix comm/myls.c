#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
  DIR *dp;
   struct dirent *dirp;
  if(argc!=2)
    {
      printf("a single argument is required\n"); exit(1);    }
   if((dp=opendir(argv[1]))==NULL)
     {
        printf("cant open the file %s\n",argv[1]);  exit(1);
     }
   while((dirp=readdir(dp))!=NULL)
    printf("%s\n",dirp->d_name);
   closedir(dp);
}
