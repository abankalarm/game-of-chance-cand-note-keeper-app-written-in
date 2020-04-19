//The output file has been changed from /tmp/notes to /var/notes, so the
//data is now stored in a more permanent place. The getuid() function is used to
//get the real user ID, which is written to the datafile on the line before the note’s
//line is written. Since the write() function is expecting a pointer for its source,
//the & operator is used on the integer value userid to provide its address.



#include<stdio.h>
#include<stdlib.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include"noteaddon.h"


void usage(char *program_name, char *filename){
    printf("usage %s < data to add to %s \n",program_name, filename);
    exit(0);
}

void fatal(char* );
void *ec_malloc(unsigned int );

int main(int argc, char *argv[]){
    int fd; //file descriptor pheww
    int userid; //every user is a assigned a specificid
    char *buffer, *datafile;


    buffer = (char*) ec_malloc(100);
    datafile= (char*) ec_malloc(20);

    strcpy(datafile, "/var/notes");


    //if arguements less than 2, display usage message and exit tada!
    if (argc<2) 
        usage(argv[0], datafile);
        
    strcpy(buffer, argv[1]);

    printf("++++++++++++++++under the hood+++++++++++++++++++\n");
    printf("\n[debug] buffer   @p stored at ", buffer);
    printf("\n[debug] datafile @p stored at ", datafile);


    strncat(buffer, "\n", 1); //add a newline on the end

    //white gandalf, open the file
    fd = open(datafile, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
    if (fd==-1)
        fatal("\nthere was an error while opening the file");
    printf("\n""[debug] file descriptor is located at %p",fd);
    
    userid = getuid(); //get the real user ID

    if (write(fd, &userid, 4)== -1) //write user ID before note daata
        fatal("in main() while writing user to the file");
    

    // lets write file
    if (write(fd, buffer, strlen(buffer))==-1)
        fatal("there was a error white writing the file");
    
    //lets close this up
    if (close(fd)==-1)
        fatal("oops theres some error closing");

    printf("the note has been saved.\n");
    free(buffer);
    free(datafile);

    
};



