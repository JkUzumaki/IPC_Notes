#include<pthread.h>
#include<sys/ipc.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<unistd.h>
#include<stdlib.h>
int id;
	
void *fun(){
	//locking
	struct sembuf asem [1];
	asem [0].sem_num = 0;
	asem [0].sem_op = -1;
	asem [0].sem_flg = 0;
	if (semop (id, asem, 1) == -1) {
		perror ("semop:buffer_count_sem");
		exit (1);
	}
	static int count = 0;
	count ++; printf("The value is %d\n", count);
	sleep(2);
	//unlocking
	asem [0].sem_num = 0;
	asem [0].sem_op = 1;
	asem [0].sem_flg = 0;
	if(semop (id, asem, 1) == -1) {
		perror ("semop: buffer_count_sem"); exit (1); 
	}
	}

int main()
{
	union semun  
	{
		int val;
		struct semid_ds *buf;
		short array [1];
	} sem_attr;

	pthread_t thread_id[10];

	int  i;
	key_t key = 110;
	sem_attr.val = 1;
	if ((id = semget (key, 1, 0660 | IPC_CREAT)) == -1) {
		perror ("semget"); exit (1);
	}
	if (semctl (id, 0, SETVAL, sem_attr) == -1) {
		perror ("semctl SETVAL"); exit (1);
	}
	for(i = 0; i < 10; i++){
		pthread_create(&thread_id[i], NULL, fun,NULL); 
	}
	for(i = 0; i < 10; i++){
		pthread_join(thread_id[i], 0);//, NULL, fun, NULL); 
	}
	
	return 0;
}
