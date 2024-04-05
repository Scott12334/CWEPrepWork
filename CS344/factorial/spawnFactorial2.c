#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t r_wait(int *stat_loc);
int main(int argc, char** argv) {
	pid_t childpid;
	int i;
	if (argc < 2 || (argc-1) % 2 == 1)
	{
		printf("Usage: %s integer1 integer2 [integer3, integer4, ...]\n", argv[0]);
		return 1;
	}
	
	for(i = 1; i < argc; i += 2)
	{
		childpid = fork();
		if (childpid == -1) {
			perror("Failed to fork");
			return 1;
		}
		if (childpid == 0) { /* child code */
			char ** myargv = (char **) calloc(sizeof(char *), 4);
			myargv[0] = "factorial2";
			myargv[1] = argv[i];
			myargv[2] = argv[i+1];
			myargv[3] = NULL;
			execv("./factorial2", myargv);
			perror("Child failed to exec factorial");
			return 1;
		}
	}
	
	while(r_wait(NULL) > 0);
	
	return 0;
}

pid_t r_wait(int *stat_loc) {
   int retval;

   while (((retval = wait(stat_loc)) == -1) && (errno == EINTR)) ;
   return retval;
}
