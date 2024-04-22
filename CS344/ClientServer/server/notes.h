#ifndef NOTES_H
#define NOTES_H
struct noteData{
	char * noteString;
}typedef NOTE;
struct noteList{
	NOTE * note;
	struct noteList * nextNode;
}typedef NOTE_LIST;

void addNote(NOTE_LIST ** noteList, char * noteMessage);
void displayNotes(NOTE_LIST * noteList);
NOTE_LIST * loadNotes(char * username);
void saveNotes(NOTE_LIST * noteList, char * username);
int noteCount(NOTE_LIST * noteList);
#endif
