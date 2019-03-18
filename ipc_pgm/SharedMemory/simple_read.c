//simple_read.c
#include<stdio.h>
#include<sys/shm.h>
#include<sys/types.h>

int main()
{
	int id;
	char *ptr;
	if((id = shmget(5, 20, IPC_CREAT|0777)) == -1){
		perror("shmget");
		return 1;
	}	
	
	ptr = shmat(id, 0, 0);
	while(1){
		puts("wait...........\n");
		puts(ptr);
		sleep(5);
	}
	shmdt(ptr);
	return 0;
}
