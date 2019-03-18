//shm_write.c
#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#define BUF_SIZE 1024
#define SHM_KEY 0x1234

//Shared memory
struct shmseg{
	int cnt;
	int complete;
	char buf[BUF_SIZE];
};

int fill_buffer(char* bufpr, int size);

int main(int argc, char *argv[])
{
	int shmid, numtimes;
	struct shmseg *shmp;
	char *bufptr;
	int spaceavailable;
	//Create shared memory
	shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644|IPC_CREAT);
	if(shmid == -1){
		perror("shmget");
		return 1;
	}
	//Attaching shared memory to the address space
	shmp = shmat(shmid, NULL, 0);
	if(shmp == (void *) -1){
		perror("Shared memory attach");
		return 1;
	}
	//assigning buf address to bufptr
	bufptr = shmp->buf;
	spaceavailable = BUF_SIZE;
	for(numtimes = 0; numtimes < 5; numtimes++){
		//returns the length of the bufptr
		shmp->cnt = fill_buffer(bufptr, spaceavailable);
		shmp->complete = 0;
		printf("Writing process: Shared memory write: Wrote %d bytes\n", shmp->cnt);
		bufptr = shmp->buf;
		spaceavailable = BUF_SIZE;
		sleep(3);
	}
	printf("Writing Process: Wrote %d times\n", numtimes);
	shmp->complete = 1;
	//detach the shared memory	
	if(shmdt(shmp) == -1){
		perror("shmdt");
		return 1;
	}
	//Control the shared memory
	if(shmctl(shmid, IPC_RMID, 0) == -1){
		perror("shmctl");
		return 1;
	}
	printf("Writing Process: Complete\n");
	return 0;
}

int fill_buffer(char* bufptr, int size)
{
	static char ch = 'A';
	int filled_count;
	//store ch data in bufptr based on size in third argument
	memset(bufptr, ch, size - 1);
	//End of the string
	bufptr[size-1] = '\0';
	if(ch > 122)
		ch = 65;
	if( (ch >= 65) && (ch <= 122) ){
		if((ch >= 91) && (ch <= 96)){
			ch = 65;
		}
	}
	filled_count = strlen(bufptr);

	ch++;
	return filled_count;
}    
