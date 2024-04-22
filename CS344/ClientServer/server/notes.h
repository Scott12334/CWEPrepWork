#ifndef NOTES_H
#define NOTES_H
struct noteData{
	char * noteTitle;
	char * noteString;
}typedef NOTE;
struct noteList{
	NOTE * note;
	struct noteList * nextNode;
}typedef NOTE_LIST;

void addNote(NOTE_LIST ** noteList, char * noteTitle, char * noteMessage);
void displayNotes(NOTE_LIST * noteList);
char *  displayNote(NOTE_LIST * noteList, char * noteTitle);
int doesNoteExist(NOTE_LIST * noteList, char * noteTitle);
NOTE_LIST * loadNotes(char * username);
void saveNotes(NOTE_LIST * noteList, char * username);
#endif
