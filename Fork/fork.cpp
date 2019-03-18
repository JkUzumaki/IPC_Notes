#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
static int i = 0;
int main()
{
	fork();
	fork();
	fork();
	fork();
	i++;
	std::cout << "	HelloWorld " << i << "\n";
	return 0;	
}
