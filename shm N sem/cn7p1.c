//file transfer by fifo or msq
#include<stdio.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<sys/sem.h>
#include<signal.h>
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
void mymsq(int pid)
{
kill(pid,SIGUSR1);
int msqid=msgget(134,IPC_CREAT|S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
struct mymsg m;
m.type=0;
int fd=open("abc.txt",O_RDONLY);
int n=0;
while(read(fd,m.msg,500)==500)
{
msgsnd(msqid,&m,sizeof(struct mymsg)-sizeof(long),0);
write(1,m.msg,500);
n++;
}
write(1,m.msg,500);
m.type=3;
msgsnd(msqid,&m,sizeof(struct mymsg)-sizeof(long),0);
printf("sending complete sent %d msg\n",n);
close(fd);
}


void myfifo(int pid)
{
char buf[500];

if(mkfifo("./xxx",S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)==-1)
perror("fifo creation error\n");
int fd1=open("abc.txt",O_RDONLY);
int fd2=open("./xxx",O_WRONLY,0);
int n;
while((n=read(fd1,buf,500))==500)
{
write(fd2,buf,500);
printf("writing to fifo\n");
}
printf("writing to fifo %d\n",n);
write(fd2,buf,n);
close(fd1);
close(fd2);
printf("completed writing to fifo\n");
kill(pid,SIGUSR2);
}




int main()
{

int shmid=shmget(777,sizeof(struct get_pid),IPC_CREAT|666);
int semid=create_sem(1);
struct get_pid* m=(struct get_pid*)shmat(shmid,NULL,0);
sem_wait(semid,0);
printf("received id %d\n",m->p);
printf("press 1 for msq transfer and 2 for fifo transfer\n");
int n;
scanf("%d",&n);
if
(n==1)
mymsq(m->p);
else
myfifo(m->p);

return 0;
}
