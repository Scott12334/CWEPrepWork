#include <stdio.h>
#include <stdlib.h>

double calc(double oper1, double oper2, char oper);
int main(int argc, char * argv[]){
	
	if(argc != 4){
	  printf("Invlaid Number of of inputs.\n");
	  printf("Usage: %s operand operator operand\n",argv[0]);	  
	  return 0;
	}
	double firstOperand = atof(argv[1]);
	double secondOperand = atof(argv[3]);
	char oper = *argv[2];
	
	double answer = calc(firstOperand, secondOperand, oper);
	printf("%.2lf %c %.2lf = %.2lf\n",firstOperand, oper, secondOperand, answer);
	return 0;
}
double calc(double oper1, double oper2, char oper){
	double answer = 0;
	switch(oper){
	   case '+':
	     answer = oper1 + oper2;
		break;
   		case '-':
		answer = oper1 - oper2;
		break;
		case '*':
		answer = oper1 * oper2;
		break;		
		case '/': if(oper2 != 0){answer = oper1/oper2;}
		break;
	}
	return answer;
}
