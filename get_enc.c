#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]){

    printf("%s environment variable is at %p", argv[1], getenv(argv[1]));
}