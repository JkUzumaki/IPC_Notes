#include<pthread.h>
#include<sys/ipc.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<unistd.h>

struct sembuf buf;

void *fun(){
	static int count = 0;
	count ++;
	printf("The value is %d\n", count);
	sleep(2);
}
	
int main()
{
	pthread_t thread_id[10];
	int id, i;
	
	for(i = 0; i < 10; i++){
		pthread_create(&thread_id[i], NULL, fun, NULL); 
	}
	for(i = 0; i < 10; i++){
		pthread_join(thread_id[i], 0);//, NULL, fun, NULL); 
	}
	
	return 0;
}
