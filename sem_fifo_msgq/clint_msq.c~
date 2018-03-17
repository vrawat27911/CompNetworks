#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<string.h>

#define SERVER_SET 24L
#define CLINT_SET 100L
#define MSGSZ 256
typedef struct massege
{
long msg_to;
char txt[MSGSZ];
}massege;

int main()
{
key_t key;
int a;
int msgid;
massege msg;
key=ftok("/home/os/gaurav",10);

if((a=msgid=msgget(key,0666|IPC_CREAT))==-1)
{printf("\n semget error");exit(1);}

if((a=msgrcv(msgid,(void*)&msg,MSGSZ,SERVER_SET,0))==-1)
{printf("\n error in msg recive");exit(-1);}
printf("\n server::\t%s",msg.txt);

msg.msg_to=CLINT_SET;
printf("\n clint::\t");
gets(msg.txt);
if(msgsnd(msgid,(void*)&msg,strlen(msg.txt)+1,0)==-1)
{printf("\n msg send  error");exit(-1);}


if(msgctl(msgid,IPC_RMID,(struct msqid_ds*)0)==-1){
printf("\n remove error  ");exit(-1);}
return 0;
}




