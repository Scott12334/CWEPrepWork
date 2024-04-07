#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int convert(char * bString);
int strlength(char * bString);
int main(int argc, char * argv[]){
	if(argc  == 1){
		printf("Not enough arguments");
		exit(0);
	}
	int answer = convert(argv[1]);
	printf("Answer is %d\n", answer);
	return 0;
}
int convert(char * bString){
       double answer = 0;
       int length = strlength(bString);
       printf("Length is %d\n",length);
       int power = 0;
       for(int i = length-1; i >=0; i--){
	       if(*(bString + i) == '1'){
       		answer += (int)pow(2,power);	       
		}
	       power++;
       }
       return answer;
}

int strlength(char * bString){
       int length = 0;
       for(int i = 0; *(bString+i) != 0; i++){
	       length ++;
       }
       return length;
}
