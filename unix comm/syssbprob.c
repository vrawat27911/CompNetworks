#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#define chairs 5
#define n 20
void * barber();
void * customer();
int ncust=0;
int semid;
 int flag=0;
union semun {
                int val;                /* value for SETVAL */
                struct semid_ds *buf;   
                ushort *array;          /* array for GETALL & SETALL */
                struct seminfo *__buf;  
                void *__pad;
        };
void sem_op(int semid,int snum,int val,int fl,int nop)
    {
          struct sembuf sbuf;
      sbuf.sem_num=snum; sbuf.sem_op=val; sbuf.sem_flg=fl; 
      if(semop(semid,&sbuf,nop)==-1)
          {printf("semop error\n");
       exit(1);}
    }
int main()
{
 pthread_t btid,ctid[n];  int i;
    semid=semget(IPC_PRIVATE,2,IPC_CREAT|0660);
      union semun values;  values.val=1;
     semctl(semid,0,SETVAL,values); values.val=0;
     semctl(semid,1,SETVAL,values); 
   pthread_create(&btid,NULL,barber,NULL);
   for(i=0;i<n;i++)
   {
     int *pn=malloc(sizeof(int));
      *pn=i;
    pthread_create(&ctid[i],NULL,customer,(void *)pn);
      sleep(rand()%1);
   }
  pthread_join(btid,NULL);
   for(i=0;i<n;i++)
    pthread_join(ctid[i],NULL);
}
void * customer(void *param)
{
  int x=*((int *)param);  
      sleep(rand()%3);
    sem_op(semid,0,-1,0,1);
   if(ncust!=chairs)
    {
         printf("customer %d added \n",x);
        ncust++;   
        if(ncust==1&&flag==1) {flag=0;sem_op(semid,1,1,0,1); }
             sem_op(semid,0,1,0,1);
    }
     else    { printf("customer %d left as shop is full\n",x);
               sem_op(semid,0,1,0,1);}
    sleep(rand()%3);
}
void * barber()
{
  while(1)
  {
    sem_op(semid,0,-1,0,1);
  if(ncust==0)
   {
      printf("barber sleeping\n");
           flag=1;
        sem_op(semid,0,1,0,1);
       sem_op(semid,1,-1,0,1);
         sem_op(semid,0,-1,0,1);
     }
     sleep(rand()%3);
    printf("cutting done..\n");
   ncust--;
     
     sem_op(semid,0,1,0,1);
  }
}
