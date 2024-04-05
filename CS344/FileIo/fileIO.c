#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

typedef struct student
{
   int id;
   char name[30];
   double gpa;
}STUDENT_TYPE;

int main(int argc, char * argv[])
{
   STUDENT_TYPE aStudent;
   char * filename = "student.bin";
   int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
   printf("Enter an id > 0 or 0 to quit: ");
   scanf("%d", &(aStudent.id));
   while (aStudent.id > 0)
   {
      printf("Enter the name: ");
      scanf("%s", aStudent.name);
      printf("Enter a gpa: ");
      scanf("%lf", &(aStudent.gpa));
      write(fd, &aStudent, sizeof(STUDENT_TYPE));
      printf("Enter an id > 0 or 0 to quit: ");
      scanf("%d", &(aStudent.id));
   }
   close(fd);

   if ((fd = open(filename, O_RDONLY)) == -1)
   {
       printf("Error trying to open file: %s\n", filename);
       return 0;
   }

   printf("Data retrieved from file:\n");
   while (read(fd, &aStudent, sizeof(STUDENT_TYPE)) == sizeof(STUDENT_TYPE))
   {
      printf("Student ID: %d\n", aStudent.id);
      printf("Student name: %s\n", aStudent.name);
      printf("Student's gpa: %lf\n", aStudent.gpa);
      printf("==========\n");
   }
   close(fd);
   return 0;
}
