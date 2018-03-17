#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<errno.h>
#define SERVER 11L
#define CLINT 25L

typedef struct message
{
long msg_to;
char msgbuffer[1000];
}msg;

struct msqid_ds buf;


int main()
{msg m;
key_t key;
key=ftok("/home/gaurav/os",5);
int msgid;
if((msgid=msgget(key,0666|IPC_CREAT))<0){
printf("\n msg queue error");exit(1);}
printf("\n msg queue have id .. %d",msgid);
while(1)
{      

	
        if((msgrcv(msgid,&m,1000,SERVER,0))==-1);
	{
	printf("\n msg recieve error");
        exit(-1);
	
	}
   	printf(" clint  :: %s", m.msgbuffer);
    	sleep(1); 
   	printf("\nserver:: \t");
  
   	gets(m.msgbuffer);
   	m.msg_to = CLINT;
    if((msgsnd(msgid,&m, strlen(m.msgbuffer)+1, 0))==-1)
    	{
       	printf("/n msg  snd  error ");
        exit(-1);
       }
}
  msgctl(msgid,IPC_RMID,&buf);


return 0;
}

