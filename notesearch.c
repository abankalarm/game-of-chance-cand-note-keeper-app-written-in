//In order for a normal user to be able to read the note data, a correspond-
//ing setuid root program is needed. The notesearch.c program will read the
//note data and only display the notes written by that user ID. Additionally, an
//optional command-line argument can be supplied for a search string. When
//this is used, only notes matching the search string will be displayed.

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include"noteaddon.h"
#define FILENAME "/var/notes"

int print_notes(int, int, char*); //function for printing note
int find_user_note(int,int); //seek in file for a note for user
int search_note(char *, char *); //search for keyword function
void fatal(char *);

int main(int argc, int *argv[]){
    int userid, printing = 1, fd;
    char searchstring[100];
    
    if (argc>1)
        strcpy(searchstring, argv[1]);
    else
    {
        searchstring[0]=0; //search string is empty
    }
    
    userid = getuid();
    fd = open(FILENAME, O_RDONLY); //open read only access only

    if (fd==-1)
    {
        fatal("in main while opening file oops");
    }

    while (printing)
    {
        printing = print_notes(fd, userid, searchstring);
    }
    printf("-----------------end of note---------------\n");
    close(fd);
}


// A function to print the notes for a given uid that match
// an optional search string;
// returns 0 at end of file, 1 if there are still more notes.
int print_notes(int fd, int uid, char *searchstring) {
    int note_length;
    char byte=0, note_buffer[100];
    note_length = find_user_note(fd, uid);
    if(note_length == -1) // If end of file reached,
        return 0;   //return 0.
    read(fd, note_buffer, note_length); // Read note data.
    note_buffer[note_length] = 0; // Terminate the string.

    if(search_note(note_buffer, searchstring)) // If searchstring found,
        printf(note_buffer);    //print the note.
    return 1;
}


int find_user_note(int fd, int user_uid){
    int note_uid=-1;
    unsigned char byte;
    int length;

    while(note_uid != user_uid){
        if(read(fd ,&note_uid, 4) != 4) //read uid data
            return -1; // if 4 bytes arent read, return end of file code
        if(read(fd, &byte, 1) != 1) //read new line seperator
            return -1;
    
        byte = length = 0;
        while (byte!="\n"){ //figure out how many bytes till end of line
            if (read(fd, &byte, 1) != 1) //read single bytee
                return -1;
        length++;
        }
    }
    lseek(fd, length * -1, SEEK_CUR); // Rewind file reading by length bytes.
    printf("[DEBUG] found a %d byte note for user id %d\n", length, note_uid);
    return length;    
}

// A function to search a note for a given keyword;
// returns 1 if a match is found, 0 if there is no match.
int search_note(char *note, char *keyword) {
    int i, keyword_length, match=0;
    keyword_length = strlen(keyword);
    if(keyword_length == 0) // If there is no search string,
        return 1;
// always "match".
    for(i=0; i < strlen(note); i++) { // Iterate over bytes in note.
        if(note[i] == keyword[match]) // If byte matches keyword,
            match++;
        // get ready to check the next byte;
        else {
    //otherwise,
            if(note[i] == keyword[0]) // if that byte matches first keyword byte,
                match = 1; // start the match count at 1.
            else
                match = 0; // Otherwise it is zero.
            }
        if(match == keyword_length) // If there is a full match,
            return 1;
                        // return matched.
    }
    return 0; // Return not matched.
}