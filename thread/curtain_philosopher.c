#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <pthread.h>

#define CURTAIN 10
#define PHILOSOPHER 10

struct Philosopher
{
    pthread_t tid;
    int id;
};

static struct Philosopher *pr;
static pthread_mutex_t curtain_locks[CURTAIN];

static void *thread_use_curtains(void *);

static void *thread_use_curtains(void *arg)
{
    struct Philosopher pr = *((struct Philosopher *)arg);
    printf("philospher %d waiting for curtains %d and %d\n", pr.id, pr.id, pr.id + 1);
    pthread_mutex_lock(&curtain_locks[pr.id]);
    pthread_mutex_lock(&(curtain_locks[(pr.id + 1) % CURTAIN]));
    printf("philospher %d uses curtains %d and %d\n", pr.id, pr.id, pr.id + 1);
    printf("philospher %d unlocks curtain %d\n", pr.id, pr.id);
    pthread_mutex_unlock(&curtain_locks[pr.id]);
    printf("philospher %d unlocks curtain %d\n", pr.id,pr.id + 1); 
    pthread_mutex_unlock(&(curtain_locks[(pr.id + 1) % CURTAIN]));
    return NULL;
}

int main()
{
    int i;

    pr = (struct Philosopher *)malloc(PHILOSOPHER * sizeof(struct Philosopher));
    for (i = 0; i < PHILOSOPHER; i++)
    {
        pr[i].id = i;
        pthread_mutex_init(&curtain_locks[i], NULL);
    }

    for (i = 0; i < PHILOSOPHER; i++){
        pthread_create(&pr[i].tid, NULL, thread_use_curtains, &pr[i]);
    }

    for (i = 0; i < PHILOSOPHER; i++){
        pthread_join(pr[i].tid, NULL);
    }

    for (i = 0; i< PHILOSOPHER; i++){
        pthread_mutex_destroy(&curtain_locks[i]);
    }

    free(pr);
    pr = NULL;
    return 0;
}