#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<sys/sem.h>
#include<sys/ipc.h>
# define nr 5
# define nw 10
int rcount=0,rwait=0;
union semun {
                int val;                
               // struct sem_id_ds *buf;   
                ushort *array;          
               // struct seminfo *__buf;  
                void *__pad;
        };
int semid;
void sem_op(int semid,int semnum,int val)
 {
   struct sembuf sb; sb.sem_num=semnum; sb.sem_flg=0; sb.sem_op=val;
   semop(semid,&sb,1) ;
 }
void * readit(void * param)
{
  int x=*((int *)param);  int t;
   
   do
  {
   t=semctl(semid,2,GETVAL,0);
   sleep(rand()%1);
  }while(t==0);
    
   sem_op(semid,0,-1);
     rcount++;
    sem_op(semid,0,1);
    printf("reading..%d\n",x);
    sleep(rand()%2);
   printf("reading..done %d\n",x);
    sem_op(semid,0,-1);
     rcount--;
    sem_op(semid,0,1);
}
void * writeit(void * param)
{  int x=*((int *)param); 
    sem_op(semid,2,-1);
      while(rcount!=0);
    
     printf("writing..%d\n",x);
    sleep(rand()%2);
   printf("writing..done %d\n",x);
    sem_op(semid,2,1);
}  
int main()
{
  pthread_t pr[nr],pw[nw];  int i,t;
   key_t key=ftok(".",12);
   semid=semget(key,3,IPC_CREAT|0666);
    if(semid==-1) printf("error in semget\n");
    union semun sn; sn.val=1;
    semctl(semid,0,SETVAL,sn);
    semctl(semid,1,SETVAL,sn);
    semctl(semid,2,SETVAL,sn);
    
  if(nr<nw) t=nr; else t=nw;
   for(i=0;i<t;i++)
   {
      int *pn=malloc(sizeof(int));
      *pn=i;
   pthread_create(&pw[i],NULL,writeit,(void *)pn);
     pthread_create(&pr[i],NULL,readit,(void *)pn); 
   }
    for(;i<nr;i++)  {
       int *pn=malloc(sizeof(int));
      *pn=i;
     pthread_create(&pr[i],NULL,readit,(void *)pn); 
    }
    for(;i<nw;i++){
      int *pn=malloc(sizeof(int));
      *pn=i;
     pthread_create(&pw[i],NULL,writeit,(void *)pn); 
    }
for(i=0;i<t;i++)
{
    pthread_join(pr[i],NULL);
   pthread_join(pw[i],NULL);
}
   for(;i<nr;i++)   pthread_join(pr[i],NULL);
    for(;i<nw;i++)  pthread_join(pw[i],NULL);
  exit(0);
}

