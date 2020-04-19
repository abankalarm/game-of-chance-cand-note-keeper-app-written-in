void *ec_malloc(unsigned int size){
    void *ptr;
    ptr = malloc(size);
    if(ptr==NULL)
        fatal("error in malloc, memory allocation");
    return ptr;
};  //error checked malloc function


void fatal(char* message){
    char error_message[100];

    strcpy(error_message, "FATAL ERROR[!!]\n");
    strncat(error_message, message, 83);
    perror(error_message);
    exit(-1);
};
       //for fatal errors lol