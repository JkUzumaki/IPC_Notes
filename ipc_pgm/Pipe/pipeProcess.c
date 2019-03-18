#include<stdio.h>
#include<unistd.h>

int main()
{
	int pipefds[2];
	// p[0] = read
	// p[1] = write
	//To check the pipe return value
	int returnstatus;
	int pid;
	char writemessages[2][20] = {"Hi", "Hello"};
	//Buffer
	char readmessage[20];
	returnstatus = pipe(pipefds);
	if(returnstatus == -1){
		printf("Unable to create pipe\n");
		return 1;
	}
	//To get the fork return value
	pid = fork();
	if(pid == 0) {
		//Child process
		//read(file descriptor, buffer, sizeof(buffer))
		read(pipefds[0], readmessage, sizeof(readmessage));
		printf("Child process - Reading from pipe - Message 1 is %s\n", readmessage);
		read(pipefds[0], readmessage, sizeof(readmessage));
		printf("Child Process -Reading from pipe - Message 2 is %s\n", readmessage);
	} else {
		//Parent process
		printf("Parent process - Writing from pipe - Message 1 is %s\n", writemessages[0]);
		//write(file description, buffer, sizeof(buffer))
		write(pipefds[1], writemessages[0], sizeof(writemessages[0]));
		printf("Parent Process - Writing from pipe - Message 2 is %s\n", writemessages[1]);
		write(pipefds[1], writemessages[1], sizeof(writemessages[1]));
	}
	return 0;
}
