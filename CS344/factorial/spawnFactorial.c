#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t r_wait(int *stat_loc);

int main(int argc, char** argv) {
        pid_t childpid;
        int i;
        if (argc < 2)
        {
                printf("Usage: %s integer1 [integer2, integer3, ...]\n", argv[0]);
                return 1;
        }

        for(i = 1; i < argc; i++)
        {
                childpid = fork();
                if (childpid == -1) {
                        perror("Failed to fork");
                        return 1;
                }
                if (childpid == 0) { /* child code */
                        execl("./factorial", "factorial", argv[i], NULL);
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
