#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "common_threads.h"

//
// Here, you have to write (almost) ALL the code. Oh no!
// How can you show that a thread does not starve
// when attempting to acquire this mutex you build?
//

typedef struct __ns_mutex_t {
    sem_t s;
} ns_mutex_t;

void ns_mutex_init(ns_mutex_t *m) {
    sem_init(&(m->s), 0, 1);
}

void ns_mutex_acquire(ns_mutex_t *m) {
    sem_wait(&(m->s));
}

void ns_mutex_release(ns_mutex_t *m) {
    sem_post(&(m->s));
}

ns_mutex_t mutex;

void *worker(void *arg) {
    int id = *((int*)arg);
    for (int i = 0; i < 5;i++)
    {
        //sleep(1);
        ns_mutex_acquire(&mutex);
        printf("I am %d\n", id);
        ns_mutex_release(&mutex);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    printf("parent: begin\n");
    ns_mutex_init(&mutex);
    pthread_t pr[3];
    int i;
    for (i = 0; i < 3; i++)
	    Pthread_create(&pr[i], NULL, worker, &i);
    for (i = 0; i < 3; i++)
	    Pthread_join(pr[i], NULL);
    printf("parent: end\n");
    return 0;
}

