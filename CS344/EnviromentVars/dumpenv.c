#include <stdio.h>
#include <stdlib.h>
int main(int argc, char * argv[], char * env[])
{
	char * envPATHVar = getenv("PATH");
	printf("%s\n\n",envPATHVar);
	while(envPATHVar != NULL){
		if(*envPATHVar == ':'){
			printf("\n");
			*envPATHVar++;
		}
		else{
			printf("%c",*envPATHVar++);
		}
	}
    return 0;
}
