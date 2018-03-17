#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>

#define SERVER 11L
#define CLINT 25L
typedef struct message
{
    long    msg_to;
    char    msgbuffer[1000];
} msg;

struct msqid_ds buf;

int main()
{
msg m;
key_t key;
key=ftok("/home/gaurav/os",5);
int msgid;
if((msgid=msgget(key,IPC_CREAT|0666))==-1){
printf("error in msgqueue");exit(-1);}
while(1){m.msg_to = SERVER;
	printf("\n clint::\t");
	gets(m.msgbuffer);
	
    if((msgsnd(msgid, &m, strlen(m.msgbuffer)+1, 0))==-1)
	{
        printf("msg snd  error  ");
        exit(-1);
   	}
    sleep(1);
    if((msgrcv(msgid, &m,1000, CLINT, 0))<0)
	 {
        printf("msg  rcv   error ");
        exit(-1);
   	 }

    
    printf(" \nserver::\t");
    printf("%s", m.msgbuffer);
}
    
    msgctl(msgid, IPC_RMID, &buf);


    
    return 0;
}

