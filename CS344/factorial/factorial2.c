#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char ** argv)
{
	int n, i, fact = 1;
	int sleepTime;
	if (argc < 3)
	{
		printf("Usage: %s n sleep_time\n", argv[0]);
		return 1;
	}
	
	n = atoi(argv[1]);
	sleepTime = atoi(argv[2]);
	for(i = 1; i <= n; i++)
	{
		fact *= i;
	}
	
	sleep(sleepTime);
	printf("%d factorial = %d\n", n, fact);
	return 0;
}