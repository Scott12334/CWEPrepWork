#include <stdio.h>
#include <stdlib.h>

void dump(double nums[], int n);
void convertToDouble(char * argv[], double nums[], int argc);
double addAll(double nums[], int n);

int main(int argc, char * argv[])
{
   if (argc == 1)
   {
      printf("No other arguments given.\n");
      printf("Usage: %s arg1 arg2 ...\n", argv[0]);
      return 0;
   }
   double nums[argc-1];
   convertToDouble(argv, nums, argc);
   dump(nums, argc-1);
   double sum = addAll(nums, argc-1);
   printf("*********************\n");
   printf("Sum of all the numbers: %lf\n", sum);
   return 0;
}//end main

void convertToDouble(char * argv[], double nums[], int argc)
{
   int i;
   for (i = 1; i < argc; i++)
   {
      nums[i-1] = atof(argv[i]);
   } 
}

void dump(double nums[], int n)
{
   int i;
   for (i = 0; i < n; i++)
   {
      printf("%lf\n", nums[i]);
   }
}

double addAll(double nums[], int n)
{
   int i;
   double sum = 0;
   for(i = 0; i < n; i++)
   {
       sum += nums[i];
   }

   return sum;
}
