#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<ucontext.h>
#include<malloc.h>
#include<string.h>
#include<sys/time.h>
#define MAX 20
#define SSIZE 32000
  int count=1;  
typedef struct mythread
{
  ucontext_t context;
  int thread_id;
    struct mythread *next;
    struct mythread *prev;
    int priority;
} mythread;
  mythread thr[MAX],Main;
   mythread *readyq,*curr_thr;
 ucontext_t ext;
typedef struct my_mutex
{
  int val;  mythread *waitq,*last; 
}my_mutex;
typedef struct my_cond
{
  mythread *waitq,*last;
}my_cond;
void switcher()
{
 // printf("schedule called\n%d",count++);
 
  mythread *temp=curr_thr;
     curr_thr=curr_thr->next; 
  swapcontext(&temp->context,&curr_thr->context);
}
void my_cond_init(my_cond *c)
{
 c->waitq=c->last=NULL; 
} 
void my_cond_wait(my_cond *c,my_mutex *m)
{
   sigset_t sig;
     sigemptyset(&sig);
    sigaddset(&sig,SIGPROF);
     sigprocmask(SIG_BLOCK,&sig,NULL);
      mythread *t;  
        t=curr_thr;
     if(curr_thr==readyq)
        readyq=readyq->next;
       if(curr_thr->next==curr_thr->prev)
            readyq=NULL;
        else
      {
       
        curr_thr=curr_thr->prev;  
          t->prev->next=t->next;
          t->next->prev=t->prev;
      }
         if(c->last==NULL)
         {    t->next=NULL;  t->prev=NULL; 
            c->waitq=t;
           c->last=t;  }
      else 
         {
            c->last->next=t;  t->prev=c->last; t->next=NULL;
             c->last=t;
         }
        switcher();
     sigprocmask(SIG_UNBLOCK,&sig,NULL);
}
int my_cond_signal(my_cond *c)
{
    if(c->waitq!=NULL)
{
    sigset_t sig;
     sigemptyset(&sig);
    sigaddset(&sig,SIGPROF);
     sigprocmask(SIG_BLOCK,&sig,NULL);
      mythread *t;  t=c->waitq;
     if(c->last==c->waitq)
        {c->waitq=c->last=NULL;}
      else  
          {
            c->waitq=t->next;  c->waitq->prev=NULL;
          }
       if(readyq!=NULL)
    {
       mythread *temp=readyq->prev;  readyq->prev=t;
           temp->next=t;  t->prev=temp;  t->next=readyq;
     }
    else
   {
     readyq=t; t->next=t;  t->prev=t;
   }
        sigprocmask(SIG_UNBLOCK,&sig,NULL);
}
   return 0;
}
int my_cond_broadcast(my_cond *c)
{
  sigset_t sig;
     sigemptyset(&sig);
    sigaddset(&sig,SIGPROF);
     sigprocmask(SIG_BLOCK,&sig,NULL);
       if(c->waitq!=NULL)
    {
     if(readyq!=NULL)
  {
        mythread *t=readyq->prev;
           t->next=c->waitq;   c->waitq->prev=t;
     mythread *temp=c->last;
     readyq->prev=temp;  temp->next=readyq;
  }
   else
   {
     readyq=c->waitq;  readyq->prev=c->last;  c->last->next=readyq;
   }
   c->waitq=c->last=NULL;
   }
        sigprocmask(SIG_UNBLOCK,&sig,NULL);
   return 0;
}
void my_mutex_init(my_mutex *m,int value)
{
  m->val=value; m->waitq=NULL;
   m->last=NULL;
}
void my_mutex_lock(my_mutex *m)
{
  printf("locked\n");
  m->val--;
   sigset_t sig;
     sigemptyset(&sig);
    sigaddset(&sig,SIGPROF);
     sigprocmask(SIG_BLOCK,&sig,NULL);
  if(m->val<0)
   {
      mythread *t;  t=curr_thr;
        curr_thr=t->prev;
         t->prev->next=t->next;
         t->next->prev=t->prev;
          
         if(m->last==NULL)
         {    t->next=NULL;  t->prev=NULL; 
            m->waitq=t;
           m->last=t;  }
      else 
         {
            m->last->next=t;  t->prev=m->last; t->next=NULL;
             m->last=t;
         }
        switcher();
     sigprocmask(SIG_UNBLOCK,&sig,NULL);
   }
}
int my_mutex_trylock(my_mutex *m)
{
  if(m->val<0)
  {return -1;}
  m->val--;
   sigset_t sig;
     sigemptyset(&sig);
    sigaddset(&sig,SIGPROF);
     sigprocmask(SIG_BLOCK,&sig,NULL);
    mythread *t;  t=curr_thr;
        curr_thr=t->prev;
         t->prev->next=t->next;
         t->next->prev=t->prev;
          
         if(m->last==NULL)
         {    t->next=NULL;  t->prev=NULL; 
            m->waitq=t;
           m->last=t;  }
      else 
         {
            m->last->next=t;  t->prev=m->last; t->next=NULL;
             m->last=t;
         }
        switcher();
     sigprocmask(SIG_UNBLOCK,&sig,NULL);
}
int my_mutex_unlock(my_mutex *m)
{
    printf("unlocked\n");
  if(m->waitq==NULL)
    return -1;
    sigset_t sig;
     sigemptyset(&sig);
    sigaddset(&sig,SIGPROF);
     sigprocmask(SIG_BLOCK,&sig,NULL);
    m->val++;
      mythread *t;  t=m->waitq;
     if(m->last==m->waitq)
        {m->waitq=m->last=NULL;}
      else  
          {
            m->waitq=t->next;  m->waitq->prev=NULL;
          }
       mythread *temp=readyq->prev;  readyq->prev=t;
           temp->next=t;  t->prev=temp;  t->next=readyq;
     
        sigprocmask(SIG_UNBLOCK,&sig,NULL);
   return 0;
}


void thr_exit();
mythread * thr_create(void* fun,void* arg)
{
  static int th_id=1;
      int i;
   for(i=0;i<MAX;i++)
      if(thr[i].thread_id==0)
           break;
   if(i==MAX)  return NULL;
     mythread *t1;
    t1=&thr[i];
     t1->thread_id=th_id++;
     getcontext(&t1->context);
      t1->context.uc_link=&ext;
       t1->context.uc_stack.ss_sp=malloc(SSIZE);
       t1->context.uc_stack.ss_size=SSIZE;
        t1->context.uc_stack.ss_flags=0;
        t1->priority=rand()%6;
          makecontext(&t1->context,(void *)fun,1,arg);
   if(readyq==NULL)
     {
     getcontext(&ext);
      ext.uc_link=&ext;
       ext.uc_stack.ss_sp=malloc(SSIZE);
       ext.uc_stack.ss_size=SSIZE;
        ext.uc_stack.ss_flags=0;
          makecontext(&ext,(void *)thr_exit,0);
         

       for(i=0;i<MAX;i++)
      if(thr[i].thread_id==0)
           break;
       curr_thr=&thr[i];
        curr_thr->thread_id=th_id++;
        readyq=curr_thr;
   curr_thr->next=curr_thr; curr_thr->prev=curr_thr; 
     
   /*    struct sigaction action;
   memset(&action,0,sizeof(action)); 
    action.sa_handler=&switcher;
     sigemptyset(&action.sa_mask);
   sigaction(SIGPROF,&action,NULL);*/
      signal(SIGPROF,switcher);
     struct itimerval timer;
    timer.it_interval.tv_sec=0;
    timer.it_interval.tv_usec=100;
    timer.it_value.tv_sec=0;
  timer.it_value.tv_usec=100;
   setitimer(ITIMER_PROF,&timer,NULL);
      }
  mythread *temp,*temp2;
     for(temp=readyq;;temp=temp->next)
      {
         if(temp->priority<t1->priority)  break;
      }
        temp2=temp->prev;   temp->prev=t1; t1->next=temp;
          temp2->next=t1;  t1->prev=temp2;
  
   return t1;
}

void thr_exit()
{
 
  curr_thr->thread_id=0;
  if(readyq->next==readyq)
  {readyq=NULL; curr_thr=NULL;}
  else
{

   if(curr_thr==readyq) readyq=readyq->next;
  mythread *temp1=curr_thr->prev;
  mythread *temp2=curr_thr->next;
  temp1->next=temp2; temp2->prev=temp1;
   curr_thr=temp2;
  setcontext(&curr_thr->context);
}
 
}
int notinq(mythread *t)
{
  if(readyq==t) return 1;
  mythread *temp=readyq->next;
   while(temp!=readyq)
   {
     if(temp==t) return 1;
     temp=temp->next;
   }
  return 0;
}
void th_join(mythread *t)
{
  while(notinq(t));
}

void f1(void* arg)
{  
    int i,j;  
  for(j=0;j<2;j++) 
{
  for(i=0;i<100000;i++) 
    printf("this is f1 thread,%d\n",*((int*)arg));
}
  
}
void f2(void* arg)
{
   int i,j;  
  for(j=0;j<2;j++) 
{
  for(i=0;i<100000;i++) 
    printf("this is f2 thread,%d\n",*((int*)arg));
}
 
}
int main()
{
   mythread *t1,*t2;
    int i;
   for(i=0;i<1;i++)
 {
   int *n=(int *)malloc(sizeof(int));
    *n=i;
   t1=thr_create(f1,(void*)n);
  if(t1==NULL)
  { printf("unable to create thread"); exit(1);}
   t2=thr_create(f2,(void*)n);
   if(t2==NULL)
    { printf("unable to create thread"); exit(1);}
  }
       
  int j;  
 for(j=0;j<1;j++) 
{
  for(i=0;i<100000;i++) 
    printf("this is main thread\n");
}

 /*for ( ; ; )
	{
		printf("This is main thread\n");
		for (i=0; i<0x200000; i++) ;
           
	}    */
  th_join(t1);
  th_join(t2);
}
