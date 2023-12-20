#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

int threshold = 10;

struct counter
{
    int counter_value_global;
    pthread_mutex_t lock_global;
    int counter_value_local[4];
    pthread_mutex_t lock_local[4];
};

struct arg
{
    struct counter * counter;
    int id;
};

struct counter counter_global;

void counter_init(struct counter * counter)
{
    counter->counter_value_global = 0;
    pthread_mutex_init(&counter->lock_global, NULL);
    for (int i = 0;i < 4; i++)
    {
        counter->counter_value_local[i] = 0;
        pthread_mutex_init(&(counter->lock_local[i]), NULL);
    }
}

void counter_increment(struct counter * counter, int i)
{
    pthread_mutex_lock(&(counter->lock_local[i]));
    counter->counter_value_local[i]++;
    if (counter->counter_value_local[i] >= threshold)
    {
        pthread_mutex_lock(&counter->lock_global);
        counter->counter_value_global += counter->counter_value_local[i];
        pthread_mutex_unlock(&counter->lock_global);
        counter->counter_value_local[i] = 0;
    }
    pthread_mutex_unlock(&(counter->lock_local[i]));
}

void* run(void * arg_test)
{
    struct arg * arg = (struct arg*)arg_test;
    struct counter* counter= arg->counter;
    int id = arg->id;
    for (int i = 0;i < 1e6;i++)
        counter_increment(counter, id);
    return NULL;
}

double time_compute(struct timeval * begin, struct timeval * end)
{
    return ((double)((end->tv_sec - begin->tv_sec) * 1e6  + (end->tv_usec - begin->tv_usec))) / 1e6 ;
}

int main(int argc, char* argv[])
{
    int val = atoi(argv[1]);
    threshold = atoi(argv[2]);
    double time_total = 0;
    pthread_t threads[val];
    struct arg args[4] = {{&counter_global, 0}, {&counter_global, 1}, {&counter_global, 2}, {&counter_global, 3}};
    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    for (int i = 0; i < val; i++)
        pthread_create(&threads[i], NULL, run, &(args[i]));
    for (int i = 0; i < val; i++)
        pthread_join(threads[i], NULL);
    gettimeofday(&end, NULL);
    time_total = time_compute(&begin, &end);
    printf("counter_global.counter_value_global:%d time_total:%f\n", counter_global.counter_value_global, time_total);
    return 0;
}
