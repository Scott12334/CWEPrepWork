#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "notes.h"


//Create note
extern void addNote(NOTE_LIST ** noteList, char * noteTitle,char * noteMessage){
	NOTE_LIST * newNote = (NOTE_LIST *)malloc(sizeof(NOTE_LIST));
	newNote->note = (NOTE *)malloc(sizeof(NOTE));
	newNote->note->noteTitle= (char *)malloc(sizeof(char)*strlen(noteTitle));
	newNote->note->noteString = (char *)malloc(sizeof(char)*strlen(noteMessage));

	strcpy(newNote->note->noteTitle,noteTitle);
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
		printf("Title: %s\n",noteList->note->noteTitle);
		printf("Note: %s\n",noteList->note->noteString);
		noteList = noteList->nextNode;
	}
}
extern int doesNoteExist(NOTE_LIST * noteList, char * noteTitle){
	while(noteList){
		if(strcmp(noteTitle,noteList->note->noteTitle) == 0){
			return 1;
		}
		noteList = noteList->nextNode;
	}
	return 0;
}
extern char *  displayNote(NOTE_LIST * noteList, char * noteTitle){
	if(doesNoteExist(noteList,noteTitle) == 0){
		return "Note does not exist";
	}
	while(noteList){
		if(strcmp(noteTitle, noteList->note->noteTitle) == 0){
			return noteList->note->noteString;
		}
		noteList = noteList->nextNode;
	}
	return "Note does not exist";
}
//Save Notes
extern void saveNotes(NOTE_LIST * noteList, char * username){
	char * fileName = (char *)malloc(sizeof(char)*(strlen(username)+6));
	strcpy(fileName,username);
	strcat(fileName,".node");
	fileName[strlen(fileName)] = 0;
	FILE * noteFile = fopen(fileName,"w");
	if(noteFile == NULL){
		printf("Error opening User's notes");
		return;
	}
	while(noteList){
		NOTE * currentNote = noteList->note;
		char * noteTitle = currentNote->noteTitle;
		char * noteString = currentNote->noteString;
		for(int i = 0; i < strlen(noteTitle); i++){
			fputc(noteTitle[i],noteFile);
		}
		fputc(1,noteFile);
		for(int i = 0; i < strlen(noteString); i++){
			fputc(noteString[i],noteFile);
		}
		fputc('0',noteFile);//Fufills requirment that notes are seperated by a 0
		noteList = noteList->nextNode;
	}
}
//Load Notes
extern NOTE_LIST * loadNotes(char * username){
	NOTE_LIST * loadedList = NULL;
	char * fileName = (char *)malloc(sizeof(char)*(strlen(username)+6));
	strcpy(fileName,username);
	strcat(fileName,".node");
	fileName[strlen(fileName)] = 0;
	FILE * noteFile = fopen(fileName,"r");

	if(noteFile == NULL){
		printf("Error opening User's notes");
		return NULL;
	}
	char * noteTitle = (char *)malloc(sizeof(char)*20);
	char * noteMessage = (char *)malloc(sizeof(char)*1000);
	int len = 0;
	int stringOn = 0;
	int c;
	
	while((c = fgetc(noteFile)) != EOF){
		if(c != 1 && c != '0'){
			if(stringOn == 0){
				noteTitle[len++] = c;
			}else if(stringOn == 1){
				noteMessage[len++] = c;
			}
		}else if(c == 1){
			noteTitle[len] = 0;
			stringOn++;
			len = 0;
		}else if(c == '0'){
			noteMessage[len] = 0;
			addNote(&loadedList,noteTitle,noteMessage);
			stringOn = 0;
			len = 0;
		}
	}
	return loadedList;
}
