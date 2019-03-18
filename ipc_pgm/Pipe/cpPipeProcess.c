#include<stdio.h>
#include<unistd.h>
//This is a half duplex communication using pipe in inter process communication
int main()
{
	int pipefds[2];
	// p[0] = read
	// p[1] = write
	//To check the pipe return value
	int returnstatus;
	int pid;
	char msg[20];
	//Buffer
	char readmessage[20];
	returnstatus = pipe(pipefds);
	if(returnstatus == -1){
		printf("Unable to create pipe\n");
		return 1;
	}
	//To get the fork return value
	pid = fork();
	while(1){
		if(pid == 0) {
			//Child process
			printf("------------------------------READ----------------------------------------\n");
			//read(file descriptor, buffer, sizeof(buffer))
			read(pipefds[0], readmessage, sizeof(readmessage));
			printf("Child process - Reading from pipe - Message 1 is %s\n", readmessage);
		} else {
			//Parent process
			printf("-------------------------------WRITE---------------------------------------\n");
			printf("Enter the message to be written: \n");
			scanf("%s",&msg);
			printf("Parent process - Writing from pipe - Message 1 is %s\n", msg);
			//write(file description, buffer, sizeof(buffer))
			write(pipefds[1], msg, sizeof(msg));
		}
	}
	return 0;
}
