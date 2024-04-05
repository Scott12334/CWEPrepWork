#include <stdio.h>
#include <string.h>
int main(int argc, char * argv[], char * env[])
{
    int i = 0;
    while(env[i])
    {
		char start[4];
		strncpy(start,env[i],4);
		if(strstr(start,"PATH") != NULL){	
			char * pathString = env[i];
			*pathString++;
			*pathString++;
			*pathString++;
			*pathString++;
			*pathString++;
			while(*pathString!=0){
				if(*pathString == ':'){
					printf("\n");
					*pathString++;
				}
				else{
					printf("%c",*pathString++);
				}
			}
		}
        i++;
    }
    return 0;
}
