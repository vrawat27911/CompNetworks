# include<stdio.h>
# include<sys/types.h>
# include<sys/ipc.h>
# include<sys/shm.h>
# include<errno.h>
int main ()
{
int server_id,clint_id;
char *server_memory,*clint_memory;
char servermsg[200];
key_t server_key=ftok("/home/gaurav/os",10);
server_id=shmget(server_key,1024,0666);
key_t clint_key=ftok("/home/gaurav/os/basic",11);
clint_id=shmget(clint_key,1024,0666); /* Make shared memory segment */
if(server_id ==-1)
perror ("Creation");
else
printf ( "Allocated . % d \n ",server_id);
if(clint_id ==-1)
perror ("Creation");
else
printf ( "Allocated . % d \n ",clint_id);
server_memory=(char*)shmat(server_id,NULL,0);
clint_memory=(char*)shmat(clint_id,NULL,0);
while(1)
{
printf("\n from server..");
printf("\t %s",server_memory);
printf("\n msg for server");
gets(servermsg);
sprintf(clint_memory,servermsg);

}
shmdt(server_memory);
shmctl(server_id ,IPC_RMID ,0) ; /* Remove segment */
shmdt(clint_memory);
shmctl(clint_id ,IPC_RMID ,0) ;

return 0;
}
