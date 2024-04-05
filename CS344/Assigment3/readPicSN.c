#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//Scott Nicholson 3 March 2024
void loadColorTable(FILE ** filePtr, unsigned char ** colorTable);
void printData(unsigned char * nonColorTable);
void saveData(unsigned char * finalPrint, char dataType[9], char packedByte);
void displayWColor(unsigned char * nonColorTable, unsigned char * colorTable, unsigned char ** finalPrint);
void printFromTable(int x, int index, unsigned char * colorTable, unsigned char ** finalPrint);
void loadRestOfFile(FILE ** filePtr, unsigned char ** nonColorTable, bool colorTable);
int main(int argc, char ** argv){
	char dataType[9];
	unsigned char * colorTable;
	unsigned char * nonColorTable;
	unsigned char * finalPrint;
	FILE * filePtr; 

	//Read in the file
	filePtr = fopen("image.pic","r");
	if(filePtr == NULL){
		printf("Error opening file\n");
		exit(1);
	}
	colorTable = (unsigned char *) malloc(33*sizeof(char));

	//Display Header Data
	if(fread(dataType, sizeof(char),8,filePtr) < 8){
		printf("Error with file format\n");
		exit(1);
	}
	dataType[8] = 0;
	printf("Header Data: %s\n",dataType);
	
	//Packed Byte
	char packedByte;
	if(fread(&packedByte, sizeof(char),1,filePtr) < 1){
		printf("Eror with file format\n");
		exit(1);
	}
	printf("Packed byte: %hhx\n",packedByte);

	if((packedByte & 0x84) == 0x84){
		loadColorTable(&filePtr, &colorTable);
		nonColorTable = (unsigned char *) malloc(24*sizeof(char));
		finalPrint = (unsigned char *) malloc(72*sizeof(char));
		loadRestOfFile(&filePtr, &nonColorTable, true);
		displayWColor(nonColorTable,colorTable, &finalPrint);
		saveData(finalPrint,dataType, packedByte);
	}
	else{
		nonColorTable = (unsigned char *) malloc(48*sizeof(char));
		loadRestOfFile(&filePtr, &nonColorTable, false);
		printData(nonColorTable);
	}
	fclose(filePtr);
	return 0;
}
void printData(unsigned char * nonColorTable){
	for(int i = 0; i < 48; i++){
		if(i != 0 && i%24==0){
			printf("\n");
		}
		else if(i != 0 && i%3 == 0){
			printf(" ");
		}
		char currentChar = *(nonColorTable+i);
		int charVal = currentChar - 0;
		printf("%02hhX",currentChar);
	}
}
void saveData(unsigned char * finalPrint, char dataType[9], char packedByte){
	FILE * saveFilePtr = fopen("screen.pic","w");
	if(saveFilePtr == NULL){
		printf("Error creating file\n");
		exit(1);
	}
	packedByte = 0x99;
	//Save the header
	fwrite(dataType, sizeof(char),8,saveFilePtr);
	//Save the packed byte
	fwrite(&packedByte, sizeof(char),1,saveFilePtr);
	//Save the color printout	
	fwrite(finalPrint, sizeof(char),72,saveFilePtr);
	fclose(saveFilePtr);
}
void displayWColor(unsigned char * nonColorTable, unsigned char * colorTable, unsigned char ** finalPrint){
	for(int i = 0; i < 24; i++){
		if(i != 0 && i%8==0){
			printf("\n");
		}
		int index = *(nonColorTable+i)-0;
		printFromTable(i,index,colorTable, finalPrint);
	}
}
void printFromTable(int x, int index, unsigned char * colorTable, unsigned char ** finalPrint){
	for(int i = 0; i < 3; i++){
		char currentChar = *(colorTable+(index*4)+i);
		printf("%hhX",currentChar);
		*(*finalPrint + i + (x*3)) = currentChar;
	}
	printf(" ");
}
void loadColorTable(FILE ** filePtr, unsigned char ** colorTable){
	for(int i = 0; i < 8; i++){
		char line[4];
		if(fread(line,sizeof(char),3,*filePtr) < 3){
			printf("Eror with file format\n");
			exit(1);
		}
		line[3] = 0;
		strcpy((*colorTable)+(i*4),line);
	}
}
void loadRestOfFile(FILE ** filePtr, unsigned char ** nonColorTable, bool colorTable){
	if(colorTable == true){
		unsigned char bytes[24];
		if(fread(bytes,sizeof(char),24,*filePtr) < 24){
			printf("Eror with file format\n");
			exit(1);
		}
		memcpy((*nonColorTable),bytes,24);
	}
	else{
		unsigned char bytes[48];
		if(fread(bytes,sizeof(char),48,*filePtr) < 48){
			printf("Eror with file format\n");
			exit(1);
		}
		memcpy((*nonColorTable),bytes,48);
	}
}
