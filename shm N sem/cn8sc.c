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


void handle_server()
{
struct qmsg q1;
q1.type=1;
q1.pid=m2->pid;
msgsnd(msqid2,&q1,sizeof(struct qmsg)-sizeof(long),IPC_NOWAIT);//storing pid of free server onto queue

if(msgrcv(msqid1,&q1,sizeof(struct qmsg)-sizeof(long),0,IPC_NOWAIT)!=-1)//checking for a waiting client in the queue
{
struct qmsg q2;
msgrcv(msqid2,&q2,sizeof(struct qmsg)-sizeof(long),0,IPC_NOWAIT);//receving the first free server in queue
m2->pid=q1.pid;//loading value of the client to be serviced in shm
printf("signalling server process\n");
kill(q2.pid,SIGUSR1);//signalling the server process for service of client
}
signal(SIGUSR1,handle_server);
sem_signal(semid,0);
}



void handle_client()
{
struct qmsg q1;

if(msgrcv(msqid2,&q1,sizeof(struct qmsg)-sizeof(long),0,IPC_NOWAIT)!=-1)//checking for free server on queue
{
struct qmsg q2,q3;
if(msgrcv(msqid1,&q2,sizeof(struct qmsg)-sizeof(long),0,IPC_NOWAIT)!=-1)//if already clients are in queue
{
q3.type=1;
q3.pid=m2->pid;
msgsnd(msqid1,&q3,sizeof(struct qmsg)-sizeof(long),IPC_NOWAIT);//adding the new client to msq
m2->pid=q2.pid;
printf("signalling server process for old client service\n");
kill(q1.pid,SIGUSR1);
}
else
{
printf("signalling server process for new client service\n");
kill(q1.pid,SIGUSR1);
}
}
else//no free server available on the queue
{
struct qmsg q4;
q4.type=1;
q4.pid=m2->pid;
printf("adding the client to queue\n");
msgsnd(msqid1,&q4,sizeof(struct qmsg)-sizeof(long),IPC_NOWAIT);//adding the new client to msq
}
signal(SIGUSR2,handle_client);
sem_signal(semid,1);
}




int main()
{
printf("service coordnator started\n");
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
m1->pid=getpid();
signal(SIGUSR1,handle_server);
signal(SIGUSR2,handle_client);

while(1);

return 0;
}


