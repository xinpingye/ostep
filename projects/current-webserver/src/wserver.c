#include <stdio.h>
#include "request.h"
#include "io_helper.h"
#include <pthread.h>
#include <limits.h>
#include <string.h>

char default_root[] = ".";
#define MAX_BUFFER_LEN 30
#define MAX_THREAD_POOL_SIZE 8
#define MAXBUF (8192)
int policy = 0;

struct buffer
{
	int array[MAX_BUFFER_LEN];
	int len;
	int len_limit;
	int use_ptr;
	int fill_ptr;
} buf;

struct item
{
	int fd;
	long size;
	char method[MAXBUF], uri[MAXBUF], version[MAXBUF];
};

struct buffer2
{
	struct item array[MAX_BUFFER_LEN];
	int len;
	int len_limit;
	int use_ptr;
	int fill_ptr;
}buf2;



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

void buffer2_init(int limit)
{
	buf2.len = 0;
	buf2.len_limit = limit;
	buf2.use_ptr = 0;
	buf2.fill_ptr = 0;

}

void put(int fd)
{
	assert(buf.len < buf.len_limit);
	buf.array[buf.fill_ptr] = fd;
	buf.len++;
	buf.fill_ptr = (buf.fill_ptr + 1) % MAX_BUFFER_LEN;
}

void put2(int fd, long size, char method[MAXBUF], char* uri[MAXBUF], char* version[MAXBUF])
{
	assert(buf2.len < buf2.len_limit);
	buf2.array[buf2.fill_ptr].fd = fd;
	buf2.array[buf2.fill_ptr].size = size;
	strcpy(buf2.array[buf2.fill_ptr].method, method);
	strcpy(buf2.array[buf2.fill_ptr].uri, uri);
	strcpy(buf2.array[buf2.fill_ptr].version, version);
	buf2.len++;
	buf2.fill_ptr = (buf2.fill_ptr + 1) % MAX_BUFFER_LEN;
}

int get()
{
	assert(buf.len > 0);
	int tmp = buf.array[buf.use_ptr];
	buf.len--;
	buf.use_ptr = (buf.use_ptr + 1) % MAX_BUFFER_LEN;
	return tmp;
}

struct item get2()
{
	assert(buf2.len > 0);
	long size_less = LONG_MAX;
	int i_less = -1;
	for (int i = buf2.use_ptr; i != buf2.fill_ptr; i = (i + 1) % MAX_BUFFER_LEN)
	{
		if (size_less < buf2.array[i].size)
		{
			size_less = buf2.array[i].size;
			i_less = i;
		}
	}
	struct item tmp = buf2.array[i_less];
	buf2.array[i_less] = buf2.array[buf2.use_ptr];
	buf2.len--;
	buf2.use_ptr = (buf2.use_ptr + 1) % MAX_BUFFER_LEN;
	return tmp;
}

void* worker(void* arg)
{
	while(1)
	{
		
		if (policy == 0)
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
		else if (policy == 1)
		{
			pthread_mutex_lock(&mutex);
			while(buf2.len == 0)
				pthread_cond_wait(&fill, &mutex);
			struct item tmp = get2();
			pthread_cond_signal(&empty);
			pthread_mutex_unlock(&mutex);
			request_handle_second(tmp.fd, tmp.method, tmp.uri, tmp.version);
			close_or_die(tmp.fd);
		}
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
    
    while ((c = getopt(argc, argv, "d:p:t:m:s:")) != -1)
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
	case 's':
		if (optarg == 'f')
			policy = 0;
		else if (optarg == 's')
			policy = 1;
		break;
	default:
	    fprintf(stderr, "usage: wserver [-d basedir] [-p port]\n");
	    exit(1);
	}

 
	buffer_init(buffer_max_size);
	buffer2_init(buffer_max_size);

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
		if (policy == 0)
		{
			while(buf.len == buf.len_limit)
				pthread_cond_wait(&empty, &mutex);
			put(conn_fd);
		}
		else if (policy == 1)
		{
			char method[MAXBUF], uri[MAXBUF], version[MAXBUF];
			memset(method, 0, sizeof(method));
			memset(uri, 0, sizeof(uri));
			memset(version, 0, sizeof(version));
			long size = request_handle_first(conn_fd, method, uri, version);
			while(buf2.len == buf2.len_limit)
				pthread_cond_wait(&empty, &mutex);
			put2(conn_fd, size, method, uri, version);
		}
		pthread_cond_signal(&fill);
		pthread_mutex_unlock(&mutex);
    }
    return 0;
}


    


 
