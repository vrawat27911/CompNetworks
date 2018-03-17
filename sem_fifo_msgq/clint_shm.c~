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
struct semid_df *buf;
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
wait(clintreadmutex);
printf("\nfrom  server..");
printf("\t %s",shared_memory);


sleep(2);

printf("\n to server..");
gets(clintmsg);
sprintf(shared_memory,clintmsg);
signal(serverreadmutex);
}
shmdt(shared_memory);
shmctl(mid,IPC_RMID,0);
return 0;
}








