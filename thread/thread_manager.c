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
    int thread_local_id;
} *thread_array;

static void *thread_work(void *);

static void *thread_work(void *arg)
{
    int local_tid = *((int *)arg);
    printf("starting thread %d\n", local_tid);
    pthread_mutex_lock(&threadMutex);
    printf("thread %d is doing some work.\n", local_tid);
    printf("thread %d finishes\n", local_tid);
    thread_done++;
    thread_array[local_tid].status = 1;
    pthread_mutex_unlock(&threadMutex);
    pthread_cond_signal(&threadDied);
    printf("signal main thread\n");
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
        thread_array[i].thread_local_id = i;
        pthread_create(&thread_array[i].id, NULL, thread_work, &thread_array[i].thread_local_id);
    }
    while (thread_collected != num_threads)
    {
        pthread_mutex_lock(&threadMutex);
        while (thread_done == 0)
        {
            pthread_cond_wait(&threadDied, &threadMutex);
        }
        for (idx = 0; idx < num_threads; idx++)
        {
            if (thread_array[idx].status == 1)
            {
                s = pthread_join(thread_array[idx].id, NULL);
                thread_array[idx].status = 2;
                printf("Reaped thread %d\n", thread_array[idx].thread_local_id);
                    thread_done--;
                    thread_collected++;
                    printf("%d remaining threads.\n",(num_threads-thread_collected));
            }
        }
        pthread_mutex_unlock(&threadMutex);
    }
    return 0;
}