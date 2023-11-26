#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

static pthread_mutex_t  mutex;
static pthread_cond_t  condition;
static int ready1 = 0;

void* func1(void* arg)
{
    pthread_mutex_lock(&mutex);
    if(ready1 == 0)
        pthread_cond_wait(&condition, &mutex);
    printf("wake!!!\n");
    pthread_mutex_unlock(&mutex);
}

void* func2(void* arg)
{
    pthread_mutex_lock(&mutex);
    ready1 = 1;
    pthread_cond_signal(&condition);
    printf("start test\n");
    for(int i = 0;i <  10000 * 10 * 100;i++)
        ready1++;
    printf("end test\n");
    pthread_mutex_unlock(&mutex);
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition, NULL);
    pthread_t t1; 
    int rc1 = pthread_create(&t1, NULL, func1, NULL);
    pthread_t t2; 
    int rc2 = pthread_create(&t2, NULL, func2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);
    pthread_cond_destroy(&condition);

    return 0;

}