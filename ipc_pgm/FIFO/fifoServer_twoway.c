#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

void reverse_string(char*);

int main()
{
	int fd;
	char readbuf[80];
	char end[10];
	int to_end;
	int read_bytes;
	//Create a fifo
	mkfifo("fifo_twoway", 0777);
	strcpy(end, "end");
	fd = open("fifo_twoway", O_RDWR);
	while(1){
		read_bytes = read(fd, readbuf, sizeof(readbuf));
		readbuf[read_bytes] = '\0';
		printf("FIFO_SERVER: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
		to_end = strcmp(readbuf, end);
		if(to_end == 0){
			close(fd);
			break;
		}
		reverse_string(readbuf);
		printf("FIFOSERVER: Sending reversing string: \"%s\" and the length is %d\n", readbuf, (int) strlen(readbuf));
		sleep(2);
	}
	return 0;
}

void reverse_string(char* str)
{
	int last, limit, first;
	char temp;
	last = strlen(str) - 1;
	limit = last / 2;
	first = 0;

	while(first < last){
		temp = str[first];
		str[first] = str[last];
		str[last] = temp;
		first++;
		last--;
	}
	return;
}
