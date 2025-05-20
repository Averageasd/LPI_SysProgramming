#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_TH 10

static pthread_cond_t threadDied = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;

static int thread_done = 0;
// static struct thread_object *thread_array[NUM_TH];
static int all_threads_join = 0;
static int thread_collected = 0;

static struct thread_object
{
    pthread_t id;
    int status;
} *thread_array;

static void *thread_work(void *);

static void *thread_work(void *arg)
{
    printf("starting thread %lu\n", pthread_self());
    int local_tid = *((int *)arg);
    pthread_mutex_lock(&threadMutex);
    printf("thread %d is doing some work.\n", local_tid);
    printf("thread %d finishes\n", local_tid);
    thread_done++;
    thread_array[local_tid].status = 1;
    pthread_mutex_unlock(&threadMutex);
    pthread_cond_signal(&threadDied);
    printf("signal parent\n");
    return NULL;
}

int main()
{
   
    int i;
    int s;
    int idx;
    int num_threads = NUM_TH;
     thread_array = malloc(sizeof(*thread_array) * NUM_TH);
    for (i = 0; i < NUM_TH; i++)
    {
        thread_array[i].status = 0;
        pthread_create(&thread_array[i].id, NULL, thread_work, &i);
    }
    while (thread_collected!=num_threads){
        printf("starting main thread %ld\n", pthread_self());
        pthread_mutex_lock(&threadMutex);
        printf("num thread done %d\n", thread_done);
        while (thread_done == 0){
            pthread_cond_wait(&threadDied, &threadMutex);
        }
        for (idx = 0; idx<num_threads;idx++){
            printf("thread %d and status of thread %d\n", idx, thread_array[idx].status);
            if (thread_array[idx].status == 1){
                s = pthread_join(thread_array[idx].id,NULL);
                if (s!=0){
                    printf("error joining thread %d\n", idx);
                }
                thread_array[idx].status = 2;
                printf("thread done %d\n", thread_done);
                printf("join thread %d\n", idx);
                if (thread_done > 0){
                    thread_done--;
                    thread_collected++;
                }
            }
        }
        printf("number of collected threads %d\n", thread_collected);
        printf("main thread unlocking thread mutex\n");
        pthread_mutex_unlock(&threadMutex);
    }
    return 0;
}