#include<stdio.h>

int main()
{
    char string[10];
    int A = -73;
    unsigned int B = 31337;

    strcpy(string, "sample");

    printf("[A] dec: %d ,hex: %x. unsigned: %u\n," A , A , A );
    printf("[B] dec: %d ,hex: %x. unsigned: %u\n,"B ,B ,B );
    printf("[field with width on B] 3: '%3u', 10: '%10u', 8: '%08u' \n", B, B, B);
    printf("[string] %s Address %08x\n", string, string);
// Example of unary address operator (dereferencing) and a %x format string
    printf("variable A is at address: %08x\n", &A);
}