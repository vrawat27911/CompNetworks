#include<sys/ipc.h>
#include<sys/sem.h>
#include<string.h>
#include<errno.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#define size 5
int buffer[size];
int pind=0,cind=0,count=0;
  int sem_id;
   int fflag=0,eflag=0;
union semun {
                int val;                /* value for SETVAL */
                struct sem_id_ds *buf;   
                ushort *array;          /* array for GETALL & SETALL */
                struct seminfo *__buf;  
                void *__pad;
        };


  void sem_op(int sem_id,int snum,int val,int fl,int nop)
    {
          struct sembuf sbuf;
      sbuf.sem_num=snum; sbuf.sem_op=val; sbuf.sem_flg=fl; 
      if(semop(sem_id,&sbuf,nop)==-1){printf("%s\n",strerror(errno));
       exit(1);}
    }
  void * producer()
   {
       while(1)
        { 
           sem_op(sem_id,0,-1,0,1);
            if(count==size)
             {  fflag=1;
                printf("producer is waiting..\n");
                 sem_op(sem_id,0,1,0,1);
                   sem_op(sem_id,2,-1,0,1);
                  fflag=0;
                   sem_op(sem_id,0,-1,0,1);
             }
            buffer[pind]=pind;  count++;  pind=(pind+1)%size;
          if(count==1 && eflag==1) {    eflag=0;sem_op(sem_id,1,1,0,1);          }
            printf("produced %d count is %d\n",buffer[pind],count);
             sem_op(sem_id,0,1,0,1);  
                  sleep(rand()%2);

        }
   }
    void * consumer()
   {
         while(1)
          {            
             sem_op(sem_id,0,-1,0,1);
                if(count==0)
                {   eflag=1;
                   printf("consumer is waiting..\n");
                   sem_op(sem_id,0,1,0,1);
                   sem_op(sem_id,1,-1,0,1);
                   sem_op(sem_id,0,-1,0,1);
                }
             count--;
               
           printf("consumed %d count is %d\n",buffer[cind],count);
               cind=(cind+1)%size;
             if(count==size-1 && fflag==1) 
     {fflag=0;      sem_op(sem_id,2,1,0,1);}
             sem_op(sem_id,0,1,0,1);
       sleep(rand()%3);
          }
   }
int main()
{
  pthread_t pid,cid;
    key_t key=ftok(".",14);
   sem_id=semget(key|IPC_PRIVATE,3,IPC_CREAT|0666);
    if(sem_id==-1) {printf("semget error\n"); exit(1);}
     union semun sn; 
     ushort arr[]={1,0,0};    sn.array=arr;
     if(semctl(sem_id,3,SETALL,sn)==-1)
      {printf("semctl error 0\n"); exit(1);}
     
   pthread_create(&pid,NULL,producer,NULL);
   pthread_create(&cid,NULL,consumer,NULL);
   pthread_join(pid,NULL);
   pthread_join(cid,NULL);
   exit(0);
}
