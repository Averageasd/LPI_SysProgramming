#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
static void * threadFunc(void *);

static void * threadFunc(void * arg){
    char *s = (char *) arg;
    printf("%s", s);
    return (void *) strlen(s);
}

int main(int argc, char *argv[]){
    pthread_t thread;
    void *res;
    int s;

    // thread will now execute this function.
    // starting point for thread
    s = pthread_create(&thread, NULL, threadFunc, "Hello world\n");
    if (s != 0){
        printf("error.\n");
        exit(1);
    }
    printf("message from main()\n");
    // wait for thread to finish
    // asign value return from threadFunc to res
    // store it at address associated with res
    s = pthread_join(thread, &res);
    if (s!=0){
        printf("error\n");
        exit(1);
    }
    printf("value returned from thread %ld\n", (long) res);
    exit(0);
}