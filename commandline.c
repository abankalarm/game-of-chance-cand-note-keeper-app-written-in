#include<stdio.h>

int main(int arg_count, char *arg_list[])
{
    int i;
    printf("there are %d arguments provided \n", arg_count);
    for(i=0;i<arg_count;i++)
        printf("argument %s", arg_list[i],"\n");

} 