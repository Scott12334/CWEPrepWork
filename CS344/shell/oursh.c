#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


int process(char *);
int countTokens(char * cmdCpy);
void setArgv(char ** argv, char * cmd);
void freeSpace(char ** argv);
void dump(char ** argv);

int main(int argc, char * argv[])
{
    char cmd[256];

    printf("Welcome to our shell.\n");

    do
    {
        printf("%% ");
        fgets(cmd, 255, stdin);
        cmd[strlen(cmd)-1] = '\0';
        

        if(strcmp("exit", cmd)==0)
        {
            printf("Bye!\n");
        } else
        {
            int status = process(cmd);
            if (status == -1)
            {
                printf("Error executing %s\n", cmd);
            }

        }
    } while(strcmp("exit", cmd)!=0);

    return 0;
}

int process( char * cmd)
{
	char ** argv;
	char cmdCpy[256];
	strcpy(cmdCpy, cmd);
	int count = countTokens(cmdCpy);
    if(count == 0)
        return 0;
    strcpy(cmdCpy, cmd);
	argv = (char **) malloc(sizeof(char *) * (count + 1));
	argv[count] = NULL;
	setArgv(argv, cmdCpy);
    dump(argv);
	
    if(strcmp("ls", argv[0]) == 0)
    {
        //printf("ls was typed\n");
        int pid = fork();
        if(pid == 0)
        {
            char cmdPath[256];
            char * path = "/bin/";  //Note: This requires work. You need to get the path
            //from the PATH environment variable.
            strcpy(cmdPath, path);
            strcat(cmdPath, argv[0]);
            execv(cmdPath, argv);
            return -1;
        }
        int status = 0;
        wait(&status);
    } else
    {
        printf("Cannot understand: %s\n", argv[0]);
    }

    freeSpace(argv);
    return 0;
}

int countTokens(char * cmdCpy)
{
	char * token;
	int count = 0;
	char * s = " ";
   /* get the first token */
   token = strtok(cmdCpy, s);
   
   /* walk through other tokens */
   while( token != NULL ) {
      count++;
    
      token = strtok(NULL, s);
   }
   
   return count;
   
	
}

void setArgv(char ** argv, char * cmd)
{
	char * token;
	char * s = " ";
    int i = 0;
   /* get the first token */
   token = strtok(cmd, s);
   
   /* walk through other tokens */
   while( token != NULL ) {
      int strlength = strlen(token);
      argv[i] = (char *) malloc(sizeof(char) * strlength + 1);
      strcpy(argv[i], token);
      argv[i][strlength] = '\0';
      token = strtok(NULL, s);
      i++;
   }

}

void freeSpace(char ** argv)
{
    int i = 0;
    while(argv[i])
    {
        free(argv[i]);
        i++;
    }
    free(argv);
}

void dump(char ** argv)
{
    int i = 0;
    while(argv[i])
    {
        printf("%s\n", argv[i]);
        i++;
    }
}
