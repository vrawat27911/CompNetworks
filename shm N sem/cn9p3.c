#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/shm.h>
#include<sys/ipc.h>
int shmid;
struct msg
{
int pid1;
int pid2;
int pid3;
char sent;
int receive;
};
struct msg* m;
int flag;
void sig_handle()
{
if(m->receive==1)
printf("the element exists in the tree\n");
else
printf("the element does not exist\n");
flag=0;
signal(SIGUSR1,sig_handle);
}


int main()
{
flag=0;
shmid=shmget(7654,sizeof(struct msg),IPC_CREAT|666);
m=(struct msg*)shmat(shmid,NULL,0);
m->pid1=getpid();
int ans;
char c;
signal(SIGUSR1,sig_handle);
while(1)
{
if(flag==0)
{
printf("enter the character to be searched\n");
scanf("%c",&c);
m->sent=c;
printf("search for element in tree 1/2\n");
scanf("%d",&ans);
flag=1;
if(ans==1)
kill(m->pid2,SIGUSR1);
else
kill(m->pid3,SIGUSR1);
}
}
return 0;
}
