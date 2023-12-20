#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

struct counter
{
    int counter_value;
    pthread_mutex_t lock;
};

struct counter counter_global;

void counter_init(struct counter * counter)
{
    counter->counter_value = 0;
    pthread_mutex_init(&counter->lock, NULL);
}

void counter_increment(struct counter * counter)
{
    pthread_mutex_lock(&counter->lock);
    counter->counter_value++;
    pthread_mutex_unlock(&counter->lock);
}

void counter_decrement(struct counter * counter)
{
    pthread_mutex_lock(&counter->lock);
    counter->counter_value--;
    pthread_mutex_unlock(&counter->lock); 
}

void* run(void * counter)
{
    for (int i = 0;i < 1e6;i++)
        counter_increment((struct counter*)counter);
    return NULL;
}

double time_compute(struct timeval * begin, struct timeval * end)
{
    return ((double)((end->tv_sec - begin->tv_sec) * 1e6  + (end->tv_usec - begin->tv_usec))) / 1e6 ;
}

int main(int argc, char* argv[])
{
    int val = atoi(argv[1]);
    double time_total = 0;
    pthread_t threads[val];
    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (int i = 0; i < val; i++)
        pthread_create(&threads[i], NULL, run, &counter_global);
    for (int i = 0; i < val; i++)
        pthread_join(threads[i], NULL);
    gettimeofday(&end, NULL);
    time_total = time_compute(&begin, &end);
    printf("counter_global.counter_value:%d time_total:%f\n", counter_global.counter_value, time_total);
    return 0;
}
