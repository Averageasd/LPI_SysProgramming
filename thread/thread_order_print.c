#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_T 20

static pthread_mutex_t locks[NUM_T];
static int num_flags[NUM_T];
static pthread_cond_t sleep_vars[NUM_T];
static pthread_t threads[NUM_T];
static int thread_arr[NUM_T];

static void *printWork(void *arg)
{
    int t_id = *((int *)arg);
    printf("thread %d starts\n", t_id);
    pthread_mutex_lock(&locks[t_id]);
    if (t_id == 0)
    {
        if (num_flags[t_id] == 0)
        {
            num_flags[t_id] = 1;
            printf("%d\n", t_id);
        }
    }
    else
    {
        while (num_flags[t_id] == 0)
        {
            pthread_cond_wait(&sleep_vars[t_id], &locks[t_id]);
        }
        printf("%d\n", t_id);
    }
    int next = t_id + 1;
    if (next < NUM_T)
    {
        pthread_mutex_lock(&locks[next]);
        if (num_flags[next] == 0)
        {
            num_flags[next] = 1;
        }
        printf("wake up next thread %d\n", next);
        pthread_mutex_unlock(&locks[next]);
        pthread_cond_signal(&sleep_vars[next]);
    }
    pthread_mutex_unlock(&locks[t_id]);
    return NULL;
}

int main()
{
    int i;

    for (i = 0; i < NUM_T; i++)
    {
        pthread_mutex_init(&locks[i], NULL);
    }

    for (i = 0; i < NUM_T; i++)
    {
        pthread_cond_init(&sleep_vars[i], NULL);
    }
    for (i = 0; i < NUM_T; i++)
    {
        thread_arr[i] = i;
    }
    for (i = 0; i < NUM_T; i++)
    {
        num_flags[i] = 0;
    }

    for (i = 0; i < NUM_T; i++)
    {
        pthread_create(&threads[i], NULL, printWork, &thread_arr[i]);
    }

    for (i = 0; i < NUM_T; i++)
    {
        pthread_join(threads[i], NULL);
        pthread_mutex_destroy(&locks[i]);
        pthread_cond_destroy(&sleep_vars[i]);
    }
    return 0;
}