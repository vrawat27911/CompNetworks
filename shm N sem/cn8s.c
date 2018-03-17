#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/msg.h>
#include<signal.h>
int shmid2;//for storing pid of the client who is going to be serviced 
int shmid3;//storing pid of server which is informing sc
int shmid1;//for storing pid of sc
int msqid1;//for maintaining queue of waiting clients by sc
int msqid2;//for maintaining queue of ready severs waiting for clients
int semid;
struct mymsg* m1,m2,m3;
union semun
{
int val;
struct semid_ds *buf;
unsigned short *array;
};

int create_sem(int num)
{
int semid;
if((semid=semget(449,num,IPC_CREAT|666))==-1)
perror("semaphore creation error\n");
int i;
union semun s;
s.val=1;
for(i=0;i<num;i++)
semctl(semid,i,SETVAL,s);
return semid;
}

void sem_wait(int semid,int num)
{
struct sembuf s;
s.sem_num=num;
s.sem_flg=0;
s.sem_op=-1;
semop(semid,&s,1);
}

void sem_signal(int semid,int num)
{
struct sembuf s;
s.sem_num=num;
s.sem_flg=0;
s.sem_op=1;
semop(semid,&s,1);
}
struct mymsg
{
int pid;
};
struct qmsg
{
long type;
int pid;//pid of client or server
};

void handle_sc()
{
printf("server pid %d is servicing client with pid %d",getpid(),m2->pid);
kill(m2->pid,SIGUSR2);
sleep(1);
signal(SIGUSR1,handle_sc);
sem_wait(semid,0);
m3->pid=getpid();
kill(m1->pid,SIGUSR1);
}


int main()
{
printf("server started\n");
msqid1=msgget(911,IPC_CREAT|S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
msqid2=msgget(912,IPC_CREAT|S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
//msqid3=msgget(913,IPC_CREAT|S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
semid=create_sem(2);

if((shmid1=shmget(211,sizeof(struct mymsg),IPC_CREAT|666))==-1)
perror("shared memory creation error\n");
m1=(struct mymsg*)shmat(shmid1,NULL,0);
if((shmid2=shmget(201,sizeof(struct mymsg),IPC_CREAT|666))==-1)
perror("shared memory creation error\n");
m2=(struct mymsg*)shmat(shmid2,NULL,0);
if((shmid3=shmget(201,sizeof(struct mymsg),IPC_CREAT|666))==-1)
perror("shared memory creation error\n");
m3=(struct mymsg*)shmat(shmid2,NULL,0);
sleep(4);
signal(SIGUSR1,handle_sc);
sem_wait(semid,0);
m3->pid=getpid();
kill(m1->pid,SIGUSR1);
while(1);
return 0;
}

