#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main()
{
	int fd, fd1;
	char readbuf[80];
	char end[10];
	int to_end;
	int read_bytes;

	//Create fifo
	fd1 = mkfifo("server", 0777);
	if(fd1 == -1){
		perror("mkfifo");
		return 1;
	}
	while(1){
		fd = open("server", O_RDONLY);
		read_bytes = read(fd, readbuf, sizeof(readbuf));
		readbuf[read_bytes] = '\0';
		printf("Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
		to_end = strcmp(readbuf, "end");
		if(to_end == 0){
			close(fd);
			break;
		}
	}
	return 0;
}
