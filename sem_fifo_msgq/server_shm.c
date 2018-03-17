#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<stdlib.h>
#include<sys/sem.h>

int serverreadmutex,serverwritemutex,clintreadmutex,clintwritemutex;



struct semon
{
int val;
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

int main()
{key_t shmkey,semkey1,semkey2,semkey3,semkey4;
int mid;
char *shared_memory;
char clintmsg[2000];
shmkey=ftok("/home/gaurav/os",5);
semkey1=ftok("/home/gaurav/os",20);
semkey2=ftok("/home/gaurav/os",45);
semkey4=ftok("/home/gaurav/os",210);
semkey3=ftok("/home/gaurav/os",204);
if((mid=shmget(shmkey,1024,0666|IPC_CREAT))==-1)
{
printf("\n memmry error");
exit(1);}
printf("\n memmory alloted havin id.. %d",mid);
if((serverwritemutex=semget(semkey1,1,0666|IPC_CREAT))==-1)
{printf("\n semaphore error");
exit(1);
}
arg.val=1;
semctl(serverwritemutex,0,SETVAL,arg);
//printf("\n semaphore having id.. %d",servermutex);
if((clintwritemutex=semget(semkey2,1,0666|IPC_CREAT))==-1)
{printf("\n semaphore error");
exit(1);
}
arg.val=0;
semctl(clintwritemutex,0,SETVAL,arg);
if((clintreadmutex=semget(semkey3,1,0666|IPC_CREAT))==-1)
{printf("\n semaphore error");
exit(1);
}
arg.val=0;
semctl(clintreadmutex,0,SETVAL,arg);
if((serverreadmutex=semget(semkey4,1,0666|IPC_CREAT))==-1)
{printf("\n semaphore error");
exit(1);
}
arg.val=0;
semctl(serverreadmutex,0,SETVAL,arg);
shared_memory=(char*)shmat(mid,NULL,0);
while(1)
{

wait(serverwritemutex);
printf("\n to clint..");
gets(clintmsg);
sprintf(shared_memory,clintmsg);
signal(clintreadmutex);
sleep(2);
wait(serverreadmutex);
printf("\n from clint..");
printf("\t %s",shared_memory);
signal(serverwritemutex);
}
shmdt(shared_memory);
shmctl(mid,IPC_RMID,0);
return 0;
}








