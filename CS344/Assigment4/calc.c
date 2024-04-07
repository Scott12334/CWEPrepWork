#include <stdio.h>
#include <stdlib.h>
//Scott Nicholson 20 March 2024
int main(int argc, char * argv[]){
	if(argc != 4){
		printf("Usage: ./calc operand1 operator operand2\n");
	}
	float operand1 = atof(argv[1]);
	float operand2 = atof(argv[3]);
	char operator = *argv[2];
	float result;
	switch(operator){
		case '*':
			result = operand1*operand2;
			break;
		case '+':
			result = operand1+operand2;
			break;
		case '-':
			result = operand1-operand2;
			break;
		case '/':
			if(operand2 != 0){
				result = operand1/operand2;
				break;
			}
			printf("You can not divide by 0\n");
			return 0;
	}
	printf("The result is: %f\n",result);
}
