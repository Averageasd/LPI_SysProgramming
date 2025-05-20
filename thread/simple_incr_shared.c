#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

static int glob = 0;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void * increNumFunc(void * arg){

    // convert void pointer to int pointer then dereference it
    int loop = *((int *) arg);
    int loc, j;
    loc = 0;
    j = 0;
    for (j = 0; j < loop; j++){
        // use lock to protect critical area
        pthread_mutex_lock(&mtx);
        loc = glob;
        loc++;
        glob = loc;
        pthread_mutex_unlock(&mtx);
    }
    return NULL;
}

int main(int argc, char *argv[]){
    pthread_t t1, t2;

    int loops, s;
    loops = 10000000;
    s = pthread_create(&t1, NULL, increNumFunc, &loops);
    s = pthread_create(&t2, NULL, increNumFunc, &loops);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("value of glob = %d\n", glob);
    return 0;
}