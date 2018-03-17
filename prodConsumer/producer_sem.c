#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<unistd.h>

union semun1
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
} arg;

void wait(int semid)
{
	struct sembuf smop={0,-1,0};

	// sleeps thread till sem_val of semaphore struct is zero
	// also semz_cnt is decremented by 1
	semop(semid,&smop,1);
}

void signal(int semid)
{
	struct sembuf smop={0,1,0};
	semop(semid,&smop,1);
}

int create_init_semph(key_t key,int initial_val)
{
	int semid;
	semid=semget(key,1,0666|IPC_CREAT);
	arg.val=initial_val;
	semctl(semid,0,SETVAL,arg);
	return semid;
}

int main()
{
	int full=create_init_semph(0x20,0);
	int empty=create_init_semph(0x40,100);
	int mutex=create_init_semph(0x60,1);

	while(1)
	{
		printf("\n full is %d  \n empty is %d ",semctl(full,0,GETVAL,0),semctl(empty,0,GETVAL,0));
		wait(empty);
		wait(mutex);
		printf("\n producer is producing");
		signal(mutex);
		signal(full);
		sleep(2);
	}

	return 0;
}

