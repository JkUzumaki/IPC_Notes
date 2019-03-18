//To set semval
#include<stdio.h>
#include<sys/sem.h>
#include<sys/ipc.h>
main(int argc, char **argv)
{
	int id;
	int semval;
	id = semget(1, 3, IPC_CREAT|0777);
	if(id == -1){
		perror("semget");
		return;
	}
	printf("Enter the value:\n");
	scanf("%d", &semval);
	semval = semctl(id, atoi(argv[1]), SETVAL,semval);
	if(semval == -1){
		perror("semval");
		return;
	}
	//printf(" The id: %i\n", id);
	//printf(" The semval i.e semctl : %d\n", semval);
	return;
}
