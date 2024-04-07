#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


int process(char *, char * []);
int countTokens(char * cmdCpy);
void setArgv(char ** argv, char * cmd);
void freeSpace(char ** argv);
void dump(char ** argv);
char * getPath(char * cmd, char * env[]);
char ** makePathArray(char * path);

int main(int argc, char * argv[], char * env[])
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
            int status = process(cmd, env);
            if (status == -1)
            {
                printf("%s is not executable\n", cmd);
            }
            else if (status == -2)
            {
                printf("Error executing %s\n", cmd);
		        exit(-1); //kill the child
            }

        }
        //printf("PID: %d - CMD: %s\n", getpid(), cmd);
    } while(strcmp("exit", cmd)!=0);

    return 0;
}

int process( char * cmd, char * env[])
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
	char * path = getPath(argv[0], env);
    if(path == NULL)
    {
        return -1;
    }
        //printf("ls was typed\n");
        int pid = fork();
        if(pid == 0)
        {
            execv(path, argv);
            return -2;
        }
        int status = 0;
        wait(&status);

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

char * getPath(char * cmd, char * env[])
{
    int i = 0;
    char * path = NULL;
    while(env[i])
    {
       path = (char *) malloc(strlen(env[i]) + 1);
       char * pathCpy = path;
       strcpy(path, env[i]);
       char * tok = strtok(path, "=");
       if(strcmp(tok, "PATH") == 0)
       {
          path = strtok(NULL, "=");
          break;
       }
       free(pathCpy);       
       i++;
    }//end while

    if(path == NULL)
        return NULL;

    //Make an array of the paths in path and attach the current
    //directory as the last element in the array so you don't have to test
    // for it separately.
    char ** pathArray = makePathArray(path);
    i = 0;
    while(pathArray[i])
    {
       char * command = (char *) malloc(strlen(pathArray[i]) 
                            + 1 + strlen(cmd) + 1);
       strcpy(command, pathArray[i]);
       strcat(command, "/");
       strcat(command, cmd);
       if (access(command, X_OK) == 0)
       {
        return command;
       }
       free(command);
       command = NULL;
       i++;
    }
    return NULL;
}

//The function creates an array of all the paths plus the current path 
//and returns the array.
char ** makePathArray(char * path)
{
    //Create space for a copy of the path parameter and assign it to a
    // character pointer variable named pathCpy.
	char * pathCpy = (char *)malloc(strlen(path)*sizeof(char));
    //Copy the path into pathCpy.
	strcpy(pathCpy,path);
    //Set an integer variable named count to 0
	int pathCount = 0;
    //Use strtok to assign the first string in pathCpy to a variable
    //named tok.
	char * tok = strtok(pathCpy, ":");
    //While tok is not NULL do:
    while(tok){
        //Increment count by 1.
 		pathCount++;
        //Get the next string using strtok
		tok = strtok(NULL, ":");
    //end while
	}
    //Copy the path back into pathCpy
	strcpy(pathCpy,path);
    //Add 2 to the count
	pathCount += 2;
    //Create space for count number of CHARACTER POINTERS and assign
    //the space to a variable named pathArray of type pointer pointer.
    char ** pathArray = (char **)malloc(sizeof(char*)*pathCount); 
    //Assign NULL to the LAST element in the pathArray.
	*(pathArray + (pathCount - 1)) = NULL;
    //Declare an integer variable named i and set it to 0.
	int i = 0;
    //Use strtok to get the first string in pathCpy and assign it to the
    //tok variable
	tok = strtok(pathCpy, ":");
    //While tok is not NULL do:
	while(tok){
        //Create space that will hold a copy of the string pointed
        //to by tok. Remember to have space for the null character.
        //Assign the space to the i-th element of pathArray.
		*(pathArray+i) = (char *)malloc(sizeof(char*)*strlen(tok)+1);
        //Copy the string pointed to by tok into the i-th element of
        //pathArray.
		strcpy(*(pathArray+i),tok);
        //Use strtok to get the next string and assign it to tok.
		tok = strtok(NULL, ":");
        //Increment i by 1.
		i++;
	}//end while

    //Create space for 3 characters and assign it to the i-th element
    //of pathArray.
	*(pathArray+i) = (char *)malloc(sizeof(char*)*3);
	char cd[3]="./";
	cd[2] = 0;
    //Copy the current directory into the i-th element of pathArray.
	strcpy(*(pathArray+i),cd);
    //Return pathArray.    
	return pathArray;
}
