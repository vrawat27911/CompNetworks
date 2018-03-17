#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#define buffsize 100
  long long nl=0,nw=0,nc=0,ll=0,cpc=0;
  char pv;
 void getCount(char c)
{
            nc++;
           if(c==' '||c=='\n'){ if(pv!=' '&&pv!='\n')nw++;
                     if(c=='\n')
           {nl++; if((nc-cpc)>ll) ll=nc-cpc; cpc=nc;}
                     } pv=c;
}
int main(int n,char *argv[])
{
  
  if((n>=2)&&((n==2)?(argv[1][0]!='-'):(1)))
  {
     int i; if(n==2)  i=1; else i=2;
        
         FILE *fp=fopen(argv[i],"r");
             if(fp==NULL)
                {
                    printf("invalid file %s",argv[i]);
                     exit(1);
                }
              char c=fgetc(fp); pv=c; 
             while(c!=EOF)
              {
                   getCount(c);
                     c=fgetc(fp); 
              }
           fclose(fp);
   if(n==2)
       printf("\n %lld %lld %lld %s\n",nl,nw,nc,argv[1]);
   else
   {
     if(argv[1][1]=='l')
           printf("\n %lld %s\n",nl,argv[2]);
      else if(argv[1][1]=='w') printf("\n %lld %s\n",nw,argv[2]);
       else if(argv[1][1]=='c'||argv[1][1]=='m') printf("\n %lld %s\n",nc,argv[2]);
  else if(argv[1][1]=='L') printf("\n %lld %s\n",ll-1,argv[2]);
   }
  }

  else 
    {
      char c=getc(stdin);
    char temp=c;
     while(c!=EOF)
     {getCount(c);temp=c; c=getc(stdin);}
       if(temp!='\n') {nw++;if((nc-cpc)>ll) ll=nc-cpc; cpc=nc;}
   if(n==2)
      {
        if(argv[1][1]=='l')
           printf("\n %lld \n",nl);
      else if(argv[1][1]=='w') printf("\n %lld \n",nw);
       else if(argv[1][1]=='c'||argv[1][1]=='m') printf("\n %lld\n",nc);
  else if(argv[1][1]=='L') printf("\n %lld \n",ll);
      }
 else  
    printf("\n%lld %lld %lld\n",nl,nw,nc);
    }
}
