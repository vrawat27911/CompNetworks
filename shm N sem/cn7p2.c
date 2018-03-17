#include<stdio.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<sys/sem.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
struct get_pid
{
int p;
};
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
s.val=0;
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
long type;
char msg[500];
};

void msq_handler()
{
int msqid=msgget(134,IPC_CREAT|S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
struct mymsg m;
printf("going to print from msq\n");
int c=0;
while(msgrcv(msqid,&m,sizeof(struct mymsg)-sizeof(long),0,0)!=-1)
{printf("%s",m.msg);
c++;
if(m.type==3)
break;
}printf("printing over printed messages%d\n",c);
}

void fifo_handler()
{
printf("going to print from fifo\n");
int fd=open("./xxx",O_RDONLY);
printf("opened file\n");
char buf[100];
int n;
while((n=read(fd,buf,100))==100)
write(1,buf,100);
write(1,buf,n);
printf("completed printing\n");
close(fd);
}

int main()
{
signal(SIGUSR1,msq_handler);
signal(SIGUSR2,fifo_handler);
int shmid=shmget(777,sizeof(struct get_pid),IPC_CREAT);
int semid=create_sem(1);
printf("run1\n");
struct get_pid* m=(struct get_pid*)shmat(shmid,NULL,0);
printf("run2\n");
int t=getpid();
printf("pid is %d\n",t);
m->p=t;
printf("run3\n");

sem_signal(semid,0);
while(1);
return 0;
}
