#include<stdio.h>
#include<sys/sem.h>
#include<sys/ipc.h>
main(int argc, char **argv)
{
	int id;
	id = semget(1, 3, IPC_CREAT|0777);
	if(id == -1){
		perror("semget");
		return;
	}
	printf(" The id: %i\n", id);
	return;
}
