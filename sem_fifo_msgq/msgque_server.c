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
char mtext [MSGSIZE];
};
int  main ()
{
int qid ;
struct message sbuf,rbuf ;
key_t the_key ;
the_key = ftok ( "/home/gaurav" , 226) ;
if ( ( qid = msgget ( the_key , PERMS | IPC_CREAT ) ) < 0 ) {
perror ( " megget ") ; exit (1) ;
}
printf ( " Creating message queue with identifier % d \n " , qid ) ;

sbuf . mtype = SERVER_MTYPE ;
while(1)
{printf("\n server:\t");
gets( sbuf . mtext) ;
if ( msgsnd ( qid , & sbuf , strlen ( sbuf . mtext ) +1 , 0) < 0) {
perror ( " msgsnd ") ; exit (1) ;
}sleep(2);
//printf ( " Sent message : %s \n " , sbuf . mtext );
if ( msgrcv ( qid , & rbuf , MSGSIZE , CLIENT_MTYPE , 0) < 0) {
perror ( " msgrcv ") ; exit (1) ;}
printf ( " \nclint : %s \n " , rbuf . mtext );
}
if ( msgctl ( qid , IPC_RMID , ( struct msqid_ds *) 0) < 0) {
perror ( " msgctl ") ; exit (1) ;}
printf ( " Removed message queue with identifier %d \n " , qid );
return 0;
}

