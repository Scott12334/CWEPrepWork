#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


long convert(char * bString);
int charToInt(char currentChar);
int strlength(char * bString);
int main(int argc, char * argv[]){
	if(argc  == 1){
		printf("Not enough arguments");
		exit(0);
	}
	for(int i = 1; i < argc; i++){
		long answer = convert(argv[i]);		
		if(answer == -1){
			printf("%s is not a valid hex number\n",argv[i]);
			continue;
		}
		printf("%s (hex) = %ld (dec)\n", argv[i],answer);
	}
	return 0;
}
long convert(char * bString){
       long answer = 0;
       int length = strlength(bString);
       int power = 0;
       for(int i = length-1; i >=0; i--){
	       char currentChar = *(bString+i);
	       int currentInt = charToInt(currentChar);
	       if(currentInt == -1){return -1;}
	       answer += currentInt * pow(16,power);
	       power++;
       }
       return answer;
}
int charToInt(char currentChar){
	int newInt = 0;
       	if(currentChar >= 97 && currentChar <=102){
		newInt = (currentChar - 'a') + 10;	       
        }
	else if(currentChar >= 65 && currentChar <=70){
		newInt = (currentChar - 'A') + 10;	       
	}
	else if(currentChar >= 48 && currentChar <= 57){
		newInt = (currentChar - '0');	       
	}
	else{
		newInt = -1;
	}
	return newInt;
}
int strlength(char * bString){
       int length = 0;
       for(int i = 0; *(bString+i) != 0; i++){
	       length ++;
       }
       return length;
}
