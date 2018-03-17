#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<errno.h>
#define n 10
#define nofeats 5
int a[n];
  int sem_id;
union semun {
                int val;                /* value for SETVAL */
                struct sem_id_ds *buf;   
                ushort *array;          /* array for GETALL & SETALL */
                struct seminfo *__buf;  
                void *__pad;
        };

struct sembuf sbuf;
  void sem_op(int sem_id,int snum,int val,int fl,int nop)
    {
      sbuf.sem_num=snum; sbuf.sem_op=val; sbuf.sem_flg=fl; 
      if(semop(sem_id,&sbuf,nop)==-1)
       {printf("semop error");
       exit(1);}
    }
void * philosopher(void *param)
  {
     int x=*((int *)param);  int i;
   
  for(i=0;i<nofeats;i++)  
    {
    //sleep(rand()%2);
    printf("%d is waiting\n",x);
    while(a[x]==1||a[(x+1)%n]==1);
      printf("%d  waiting over\n",x);
     a[x]=1; a[(x+1)%n]=1;
     sem_op(sem_id,x,-1,0,1);
     sem_op(sem_id,(x+1)%n,-1,0,1);
    
     printf("philosopher %d eating\n",x);
    sleep(rand()%2);
     printf("philosopher %d thinking\n",x);
      a[x]=0; a[(x+1)%n]=0;
      sem_op(sem_id,x,1,0,1);
     sem_op(sem_id,(x+1)%n,1,0,1);
    
    //sleep(rand()%3);
   }
      printf("philosopher %d eating done\n",x);
    
  }
int main()
{
  pthread_t phil[n];
   int i;
    key_t key=ftok(".",12);
   sem_id=semget(key,n,IPC_CREAT|0666);
    if(sem_id==-1) {printf("semget error\n"); exit(1);}
    union semun sn;   ushort g[n];
     for(i=0;i<n;i++){g[i]=1; a[i]=0;}
       sn.val=1;
      for(i=0;i<n;i++)
    {
       if(semctl(sem_id,i,SETVAL,sn)==-1)
     {printf("semctl error\n i is %d",i); exit(1);}
    }
     for(i=0;i<n;i++)
    {
      int *pn=malloc(sizeof(int));
      *pn=i;
    pthread_create(&phil[i],NULL,philosopher,pn);
    
    }
    for(i=0;i<n;i++)
    pthread_join(phil[i],NULL);
  exit(0);
}
