#include<pthread.h>
#include<sys/ipc.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
int id;
	
void *fun()
{
	//locking
	struct sembuf asem ;
	asem.sem_num = 0;
	asem.sem_op = -1;
	asem.sem_flg = 0;
	if (semop (id, &asem, 1) == -1) {
		perror ("semop:buffer_count_sem");
		exit (1);
	}
	static int count = 0;
	count ++; printf("The value is %d\n", count);
	sleep(2);
	//unlocking
	asem.sem_num = 0;
	asem.sem_op = 1;
	asem.sem_flg = 0;
	if(semop (id, &asem, 1) == -1) {
		perror ("semop: buffer_count_sem");
		exit (1); 
	}
}

int main()
{
	union semun  
	{
		int val;
		struct semid_ds *buf;
		short array [1];
	} sem_attr, sem_2;
	struct semid_ds buffer;
	struct ipc_perm bb;
	sem_2.buf = &buffer;
	pthread_t thread_id[10];

	int  i;
	key_t key = 110;
	sem_attr.val = 1;
	if ((id = semget (key, 2, 0660 | IPC_CREAT)) == -1) {
		perror ("semget"); exit (1);
	}
	if (semctl (id, 0, SETVAL, sem_attr) == -1) {
		perror ("semctl SETVAL"); exit (1);
	}
	//semctl (id, 0, GETALL, sem_attr);
	//printf("bugi bfr thrd\n");
	for(i = 0; i < 3; i++){
		pthread_create(&thread_id[i], NULL, fun,NULL); 
	}
	//printf("bug aft thrd\n");
	
	//for(i = 0; i < 10; i++){
	for(i = 0; i < 3; i++){
		pthread_join(thread_id[i], 0);//, NULL, fun, NULL); 
	}
	semctl (id, 0, IPC_STAT, sem_2);
	time_t time = sem_2.buf->sem_otime;
	printf("Last semaphore time : %s\n", ctime(&time));
	printf("The number of semaphore: %lu\n",sem_2.buf->sem_nsems); 
	return 0;
}
