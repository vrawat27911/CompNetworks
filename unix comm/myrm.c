#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
  if(argc==2)
   {
       if((remove(argv[1]))==0)
         printf("\n removed");
        else printf("\n an error occured");
   }
   else printf("\n mismatch in number of arguments");
}
