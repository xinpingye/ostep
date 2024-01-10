#include <stdio.h>
#include "request.h"
#include "io_helper.h"
#include <pthread.h>

char default_root[] = ".";
#define MAX_BUFFER_LEN 30
#define MAX_THREAD_POOL_SIZE 8

struct buffer
{
	int array[MAX_BUFFER_LEN];
	int len;
	int len_limit;
	int use_ptr;
	int fill_ptr;
} buf;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;

void buffer_init(int limit)
{
	buf.len = 0;
	buf.len_limit = limit;
	buf.use_ptr = 0;
	buf.fill_ptr = 0;
}

void put(int fd)
{
	assert(buf.len < buf.len_limit);
	buf.array[buf.fill_ptr] = fd;
	buf.len++;
	buf.fill_ptr = (buf.fill_ptr + 1) % MAX_BUFFER_LEN;
}

int get()
{
	assert(buf.len > 0);
	int tmp = buf.array[buf.use_ptr];
	buf.len--;
	buf.use_ptr = (buf.use_ptr + 1) % MAX_BUFFER_LEN;
	return tmp;
}

void* worker(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(buf.len == 0)
			pthread_cond_wait(&fill, &mutex);
		int fd = get();
		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&mutex);
		request_handle(fd);
		close_or_die(fd);
	}
}

void threads_init(int threads_nums, pthread_t * pool)
{
	for (int i= 0; i < threads_nums; i++)
		pthread_create(&pool[i], NULL, worker, NULL);
}

//
// ./wserver [-d <basedir>] [-p <portnum>] 
// 
int main(int argc, char *argv[]) {
    int c;
    char *root_dir = default_root;
    int port = 10000;
	int threads_nums = 4;
	int buffer_max_size = 10;
	pthread_t thread_pool[MAX_THREAD_POOL_SIZE];
    
    while ((c = getopt(argc, argv, "d:p:t:m:")) != -1)
	switch (c) {
	case 'd':
	    root_dir = optarg;
	    break;
	case 't':
	    threads_nums = atoi(optarg);
	    break;
	case 'm':
	    buffer_max_size = atoi(optarg);
	    break;
	case 'p':
	    port = atoi(optarg);
	    break;
	default:
	    fprintf(stderr, "usage: wserver [-d basedir] [-p port]\n");
	    exit(1);
	}

 
	buffer_init(buffer_max_size);

	threads_init(threads_nums, thread_pool);

    // run out of this directory
    chdir_or_die(root_dir);

    // now, get to work
    int listen_fd = open_listen_fd_or_die(port);
    while (1) {
		struct sockaddr_in client_addr;
		int client_len = sizeof(client_addr);
		int conn_fd = accept_or_die(listen_fd, (sockaddr_t *) &client_addr, (socklen_t *) &client_len);
		pthread_mutex_lock(&mutex);
		while(buf.len == buf.len_limit)
			pthread_cond_wait(&empty, &mutex);
		put(conn_fd);
		pthread_cond_signal(&fill);
		pthread_mutex_unlock(&mutex);
    }
    return 0;
}


    


 
