# include<sys/ipc.h>
# include<sys/types.h>
# include<sys/msg.h>
# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# define MSGSIZE 128
# define PERMS 0666
# define SERVER_MTYPE 27L
# define CLIENT_MTYPE 42L
struct message {
long mtype ;
char mtext [MSGSIZE]; };
int main()
{
int qid ;
struct message sbuf , rbuf ; key_t the_key ;
the_key = ftok ("/home/gaurav" , 226) ;
if ((qid = msgget(the_key,PERMS|IPC_CREAT)) < 0 ) {
printf( " megget ") ; exit (1) ; }
printf( "Accessing message queue with identifier % d \n " ,qid);
while(1)
{
if ( msgrcv ( qid , & rbuf , MSGSIZE , SERVER_MTYPE , 0) < 0) {
perror ( " msgrcv ") ; exit (1) ;}
printf ( "\n server:: : %s  " , rbuf . mtext );
sleep(2);
sbuf . mtype = CLIENT_MTYPE ;
printf("\n clint::\t");
gets ( sbuf . mtext) ;
if ( msgsnd ( qid , & sbuf , strlen ( sbuf . mtext ) +1 , 0) < 0) {
perror ( " msgsnd ") ; exit (1) ;
}
//printf ( " Sent message : %s \n " , sbuf . mtext );
}
return 0;
}
