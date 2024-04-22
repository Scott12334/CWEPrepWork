#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "notes.h"

//Count notes
extern int noteCount(NOTE_LIST * noteList){
	int count = 0;
	while(noteList){
		count++;
		noteList = noteList->nextNode;
	}
	return count;
}
//Create note
extern void addNote(NOTE_LIST ** noteList,char * noteMessage){
	NOTE_LIST * newNote = (NOTE_LIST *)malloc(sizeof(NOTE_LIST));
	newNote->note = (NOTE *)malloc(sizeof(NOTE));
	newNote->note->noteString = (char *)malloc(sizeof(char)*strlen(noteMessage));

	strcpy(newNote->note->noteString,noteMessage);
	newNote->nextNode = NULL;

	if(*noteList == NULL){
		*noteList = newNote;
		return;
	}
	NOTE_LIST * fullNoteList = *noteList;
	while(fullNoteList->nextNode){
		fullNoteList = fullNoteList->nextNode;
	}
	fullNoteList->nextNode = newNote;
}
//Display all notes
extern void displayNotes(NOTE_LIST * noteList){
	while(noteList){
		printf("%s\n",noteList->note->noteString);
		noteList = noteList->nextNode;
	}
}
//Save Notes
extern void saveNotes(NOTE_LIST * noteList, char * username){
	char * fileName = (char *)malloc(sizeof(char)*(strlen(username)+6));
	strcpy(fileName,username);
	strcat(fileName,".note");
	fileName[strlen(fileName)] = 0;
	FILE * noteFile = fopen(fileName,"w");
	if(noteFile == NULL){
		printf("Error opening User's notes\n");
		return;
	}
	while(noteList){
		NOTE * currentNote = noteList->note;
		char * noteString = currentNote->noteString;
		for(int i = 0; i < strlen(noteString); i++){
			fputc(noteString[i],noteFile);
		}
		fputc('\n',noteFile);//Seperates the notes
		noteList = noteList->nextNode;
	}
	printf("Saved Notes to file %s\n",fileName);
	fflush(noteFile);
	fclose(noteFile);
}
//Load Notes
extern NOTE_LIST * loadNotes(char * username){
	NOTE_LIST * loadedList = NULL;
	char * fileName = (char *)malloc(sizeof(char)*(strlen(username)+6));
	strcpy(fileName,username);
	strcat(fileName,".note");
	fileName[strlen(fileName)] = 0;
	FILE * noteFile = fopen(fileName,"r");

	if(noteFile == NULL){
		printf("Error opening User's notes");
		return NULL;
	}
	char * noteMessage = (char *)malloc(sizeof(char)*1000);
	int len = 0;
	int c;
	
	while((c = fgetc(noteFile)) != EOF){
		if(c != '\n'){
			noteMessage[len++] = c;
		}else{
			noteMessage[len] = 0;
			addNote(&loadedList,noteMessage);
			len = 0;
		}
	}
	return loadedList;
}
