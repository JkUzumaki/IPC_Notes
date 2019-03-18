//simple_send.c
#include<stdio.h>
#include<sys/shm.h>
#include<sys/types.h>

int main()
{
	int id;
	char str[50];
	char *ch;
	if( (id = shmget(5, 20,IPC_CREAT|0777)) == -1){
		perror("shmget");
		return 1;
	}
	ch = shmat(id, 0, 0);
	if(ch == -1){
		perror("shmat");
		//printf("Error as shamt returned -1\n");
		return 1;
	}
	while(1){
		puts("Enter the message\n");
		gets(ch);
	}
	return 0;		
}
