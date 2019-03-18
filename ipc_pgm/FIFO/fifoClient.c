#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main()
{
	int fd, fd1;
	int end_process;
	int stringlen;
	char readbuf[80];
	char end_str[5];
	printf("FIFO_CLIENT: send messages, infinitely, to end enter \"end\"\n");
	fd = open("server", O_CREAT|O_WRONLY);
	strcpy(end_str, "end");

	while(1){
		printf("Enter sring: ");
		fgets(readbuf, sizeof(readbuf), stdin);
		stringlen = strlen(readbuf);
		readbuf[stringlen -1 ] = '\0';
		end_process = strcmp(readbuf, end_str);
		//printf
		if(end_process != 0){
			write(fd, readbuf, strlen(readbuf));
			printf("Sent sring: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
		} else {
			write(fd, readbuf, strlen(readbuf));
			printf("Sent string: \"%s\" and the string length is %d\n", readbuf, (int)strlen(readbuf));
			close(fd);
			break;
		}	
	}
	return 0;
}

