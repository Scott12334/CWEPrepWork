#include<stdio.h>
char determineLetterGrade(float totalGrade);
int main(int argc, char ** argv){
    float homeworkGrade, examOneGrade, examTwoGrade, finalGrade, feedBackGrade;
    //Homework .22
    printf("What is your average homework grade?");
    scanf("%f", &homeworkGrade);
    //Exam .2
    printf("What is your Exam One grade?");
    scanf("%f", &examOneGrade);
    //Exam II .25
    printf("What is your Exam Two grade?");
    scanf("%f", &examTwoGrade);
    //Final .3
    printf("What is your Final grade?");
    scanf("%f", &finalGrade);
    //Feedback .03
    printf("What is your Feedback grade?");
    scanf("%f", &feedBackGrade);

    float totalGrade = (homeworkGrade*.22) + (examOneGrade*.2) + (examTwoGrade*.25) + (finalGrade*.30) + (feedBackGrade*.03);
    char letterGrade = determineLetterGrade(totalGrade);
    printf("Final Letter Grade: %c", letterGrade);
    return 0;
}

char determineLetterGrade(float totalGrade){
    char letterGrade = 0;
    if(totalGrade >= 90){letterGrade = 65;}
    else if(totalGrade >= 80){letterGrade = 66;}
    else if(totalGrade >= 70){letterGrade = 67;}
    else if(totalGrade >= 60){letterGrade = 68;}
    else{letterGrade = 69;}
    return letterGrade;
}
