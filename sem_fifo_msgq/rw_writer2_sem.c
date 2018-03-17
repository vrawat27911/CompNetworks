#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<stdlib.h>
#include<pthread.h>

int write_mutex,write_block,read_block;
int reader=0,writer=0;

union semon
{int val;
struct semid_ds *buf;
unsigned short *array;
}arg;

void wait(int semid)
{

struct sembuf smop={0,-1,0};
semop(semid,&smop,1);
}

void signal(int semid)
{

struct sembuf smop={0,1,0};
semop(semid,&smop,1);
}

void *reader(void* arg)
{int i=*(int*)arg+1;
while(1)
{wait(mutex);
while(readercount<0)
wait(database);
readercount++;
signal(database);
printf("\n reader %d is reading",i);
wait(mutex);
readercount--;
if(readercount==0)
signal(database);
signal(mutex);
sleep(2);
}
}

void* writer(void *arg)
{int i=*(int*)arg+1;
while(1)
{wait(mutex);
while(readercount!=0)
wait(database);
readercount--;
signal(mutex);
printf("\n writer %d is writing",i);
wait(mutex);
readercount==0;
signal(database);
signal(mutex);
sleep(3);
}
}

int main()
{int i;
key_t key1,key2,key3;
key1=ftok("/home/gaurav",1);
key2=ftok("/home/gaurav",5);
key2=ftok("/home/gaurav/os",5);
if((read_block=semget(key1,1,0666|IPC_CREAT))<0)
printf("\n semget error");
if((write_block=semget(key2,1,0666|IPC_CREAT))<0)
printf("\n semget error");
if((write_mutex=semget(key3,1,0666|IPC_CREAT))<0)
printf("\n semget error");
arg.val=1;
semctl(read_block,0,SETVAL,arg);
arg.val=1;
semctl(write_block,0,SETVAL,arg);
arg.val=1;
semctl(write_mutex,0,SETVAL,arg);
pthread_t r[5],w[5];

for(i=0;i<5;i++)
{pthread_create(&r[i],NULL,reader,(void*)&i);
sleep(2);}
for(i=0;i<5;i++)
{pthread_create(&w[i],NULL,writer,(void*)&i);
sleep(2);}
for(i=0;i<5;i++)
{pthread_join(r[i],NULL);}
return 0;
}


