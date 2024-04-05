#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

int main(int agrc, char * argv[]){
	int x = 1;
	printf("Before the fork x is %d\n",x);
	int pid  = fork();

	if(pid < 0){
		printf("Error spawning child\n");
		return 1;
	}
	if(pid == 0){
		x++;
		printf("This is the child with value: %d\n",x);
		printf("Process ID: %d, Parent Process ID: %d\n",getpid(), getppid());
		return 0;
	}
	int status;
	wait(&status);
	printf("This is the parent with value: %d\n",x);
	printf("Process ID: %d, Parent Process ID: %d\n",getpid(), getppid());
	return 0;
}
