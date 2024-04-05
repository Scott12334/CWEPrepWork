#include <stdio.h>
#include <stdlib.h>

typedef struct bitmap{
	unsigned int width;
	unsigned int height;
	char ** imagePtr;
}BIT_MAP;

unsigned char readfile(char *, BIT_MAP *);
void skipLine(FILE *);
void displayBitMap(BIT_MAP *);

int main(int argc, char ** argv){
	BIT_MAP binaryBitMap;
	char filename[26];

	printf("Enter Binary Bit Map File Name: ");
	scanf("%25s", filename);
	char junk[1024];
	fgets(junk, sizeof(junk), stdin);

	if(readfile(filename, &binaryBitMap) != 0){
		printf("Error reading file.\n");
		return 1;
	}
	displayBitMap(&binaryBitMap);
	
	return 0;
}

unsigned char readfile(char * filename, BIT_MAP * pbm){
	FILE * fileStream;
	char magicNum[3];
	char comment[2];
	unsigned int width;
	unsigned int height;
	unsigned int i;
	if((fileStream = fopen(filename, "r")) == NULL){
		printf("Error reading file\n");
		return 1;
	}
	if(fscanf(fileStream, "%s", magicNum) != 1){
		printf("Error reading file\n");
		fclose(fileStream);
		return 2;
	}
	if(fscanf(fileStream, "%s", comment) != 1 || comment[0] != '#'){
		printf("Incorrect file format\n");
		fclose(fileStream);
		return 3;
	}
	skipLine(fileStream);
	if(fscanf(fileStream, "%u %u", &width, &height) != 2){
		printf("Incorrect file format\n");
		fclose(fileStream);
		return 4;
	}
	pbm->width = width;
	pbm->height = height;

	pbm->imagePtr = (char **) calloc(height+1, sizeof(char *));

	for(i = 0; i < height; i++){
		char * str = (char *) calloc(width+1, sizeof(char));
		if(fscanf(fileStream, "%s", str) != 1){
			printf("Error Reading data\n");
			fclose(fileStream);
		}
		pbm->imagePtr[i] = str;
	}
	pbm->imagePtr[i] = NULL;
	
	fclose(fileStream);
	return 0;
}

void skipLine(FILE* fs){
	char c;
	do{
		c = fgetc(fs);
	}while(c != '\n');
}
void displayBitMap(BIT_MAP * pbm){
	char ** image = pbm->imagePtr;
	int i =0;
	while(image[i]){
		char * str = image[i];
		for(;*str;str++){
			if(*str=='1')
				printf("*");
			else
				printf(" ");
		}
		printf("\n");
		i++;
	}
}
