#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/sem.h>
int t,p,m,tp,tm,pm;
union semon
{int val;
struct semid_ds *buf;
unsigned short *array;
}arg;

void signal(int semid)
{
struct sembuf smop={0,1,0};
semop(semid,&smop,1);
}
void wait(int semid)
{
struct sembuf smop={0,-1,0};
semop(semid,&smop,1);
}
void smokerwithtobacco()
{
while(1)
{wait(pm);
printf("\n smoker with tobacco is smoking....");
signal(t);
sleep(1);
}}
void smokerwithpaper()
{
while(1)
{wait(tm);
printf("\n smoker with paper is smoking....");
signal(p);
sleep(1);

}}
void smokerwithmatch()
{
while(1)
{wait(tp);
printf("\n smoker with match is smoking....");
signal(m);
sleep(1);

}}

void arbiter()
{while(1)
{
wait(t);
signal(pm);
sleep(1);
wait(p);
signal(tm);
sleep(1);
wait(m);
signal(tp);
sleep(1);
}
}

int main()
{int i;
pid_t pid;
key_t key[6];
for(i=0;i<6;i++)
key[i]=ftok("/home/gaurav",i);
if((p=semget(key[0],1,0666|IPC_CREAT))<0)
printf("\n error  in paper");
arg.val=1;
semctl(p,0,SETVAL,arg);
if((t=semget(key[1],1,0666|IPC_CREAT))<0)
printf("\n error in tobacco");
arg.val=1;
semctl(t,0,SETVAL,arg);
if((m=semget(key[2],1,0666|IPC_CREAT))<0)
printf("\n error in match");
arg.val=1;
semctl(m,0,SETVAL,arg);
if((tm=semget(key[3],1,0666|IPC_CREAT))<0)
printf("\n sorry tobacco and match not available");
arg.val=1;
semctl(tm,0,SETVAL,arg);
if((tp=semget(key[4],1,0666|IPC_CREAT))<0)
printf("\n tobacco and paper not available");
arg.val=1;
semctl(tp,0,SETVAL,arg);
if((pm=semget(key[5],1,0666|IPC_CREAT))<0)
printf("\n paper match not available");
arg.val=1;
semctl(pm,0,SETVAL,arg);


if((pid=fork())==0)
smokerwithtobacco();
else if((pid=fork())==0)
smokerwithpaper();
else if((pid=fork())==0)
smokerwithmatch();
if((pid=fork())==0)
arbiter();
waitpid(pid,NULL,0);
return 0;
}






