#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define BLOCK 8
#define USERS 32

struct User
{
    pthread_t t_id;
    int id;
    int block_id;
};

static int blocks[BLOCK];
static pthread_mutex_t block_locks[BLOCK];
static struct User *users;

static void *thread_use_block(void *);

static void *thread_use_block(void *arg)
{
    struct User user = *((struct User *)arg);
    printf("user %d waiting to use block %d\n", user.id, user.block_id);
    pthread_mutex_lock(&block_locks[user.block_id]);
    printf("user %d uses block %d\n", user.id, user.block_id);
    blocks[user.block_id]+=1;
    pthread_mutex_unlock(&block_locks[user.block_id]);
    return NULL;
};

int main()
{
    int i;
    users = (struct User *)malloc(USERS * sizeof(struct User));
    srand(time(NULL));

    for (i = 0; i < BLOCK; i++)
    {
        blocks[i] = 0;
        pthread_mutex_init(&block_locks[i], NULL);
    }

    for (i = 0; i < USERS; i++)
    {
        users[i].id = i;
        users[i].block_id = rand() % (BLOCK);
    }

    for (i = 0; i < USERS; i++)
    {
        pthread_create(&users[i].t_id, NULL, thread_use_block, &users[i]);
    }

    for (i = 0; i < USERS; i++)
    {
        pthread_join(users[i].t_id, NULL);
    }

    for (i = 0; i < BLOCK;i++){
        pthread_mutex_destroy(&block_locks[i]);
    }

    free(users);
    users = NULL;
    
    for (i = 0; i < BLOCK; i++){
        printf("block %d used %d times\n", i, blocks[i]);
    }

    return 0;
}