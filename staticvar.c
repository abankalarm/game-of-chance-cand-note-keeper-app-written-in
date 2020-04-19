#include<stdio.h>

void function(){
    int var = 5;
    static int static_var = 5;

    printf("\t[in function] var @p %p =  %d  ",&var, var);
    printf("\t[in function] static var @p %p = %d \n",&static_var, static_var);
    var ++;
    static_var++; //static variable ddeefined only once
}

int main() {
    int i;
    static int static_var= 1337;


    for (i=0;i<5;i++)
        {
            printf("[in main] static var &p %p is %d \n", &static_var, static_var);
            function();
        }
}