#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int pid1, pid2, pid3;
    pid1 = fork();
    pid2 = fork(); 
    pid3 = fork();
    printf("Hello from pid %d\n", getpid());
    return 0;
}