#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[]){
	int n, fact = 1;
	if(argc < 2){
		printf("Usage: %s integer\n", argv[0]);
		return 1;
	}
	printf("Process ID: %d before calculating factorial \n", getpid());
	n = atoi(argv[1]);
	for(int i = 1; i <= n; i++){
		fact *= i;
	}
	sleep(3);
	printf("Porcess ID %d: %d factorial = %d\n", getpid(), n, fact);
	return 0;
}
