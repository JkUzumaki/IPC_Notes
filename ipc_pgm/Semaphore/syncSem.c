#include<stdio.h>
#include<sys/sem.h>
#include<sys/ipc.h>

int main(int argc, char ** argv)
{
	int id, semval;
	struct sembuf v;
	//To create semaphore
	id = semget(1,3, IPC_CREAT|0777);
	if(id == -1){
		perror("semget");
		return 1;
	}
	printf("ARI Chen\n");
	v.sem_num = 0;
	v.sem_op = 0;
	v.sem_flg = 0;
	//To perform operation semaphore
	semop(id, &v, 1);
	printf("ARI Hyd\n");
	return 0;
}
