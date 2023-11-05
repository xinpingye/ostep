#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <sys/time.h>
#include <x86intrin.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>


#pragma intrinsic(__rdtsc)
int main()
{
    //struct timeval val1,val2;
    //int ret1 = gettimeofday(&val1, NULL);
    //int ret2 = gettimeofday(&val2, NULL);
    //unsigned long int  ret1 = __rdtsc();
    //gettimeofday(&val1, NULL);
    //unsigned long int ret2 = __rdtsc();
    //printf("ret1:%ld ret2:%ld\n",ret1,ret2);
    //if(ret1 == 0 && ret2 == 0)
    //{
    //    int ms = val2.tv_usec - val1.tv_usec;
    //    printf("one gettimeofday consume %d ms\n",ms);
    //    return 0;
    //}
    int fd1[2];
    pipe(fd1);
    int fd2[2];
    pipe(fd2);
    cpu_set_t set;
    CPU_ZERO(&set);
    int rc = fork();
    if(rc == 0)
    {
        CPU_SET(0, &set);
        sched_setaffinity(getpid(), sizeof(set), &set);
        close(fd1[0]);
        close(fd2[1]);
        char msg[20] = "message";
        char buf[20] = "";
        int time1 = __rdtsc();
        for(int i = 0;i< 5; i++)
        {
            write(fd1[1], msg,strlen(msg));
            //printf("child write end!!!\n");
            read(fd2[0], buf,strlen("message"));
            printf("child recv mesasage: %s\n",buf);
        }
        int time2 = __rdtsc();
        printf("time of 10 context switch : %d \n",time2 - time1);
    }
    else
    {   
        CPU_SET(0, &set);
        sched_setaffinity(getpid(), sizeof(set), &set);
        close(fd1[1]);
        close(fd2[0]);
        char buf[20] = "";
        for(int i =0;i<5; i++)
        {
            read(fd1[0],buf,strlen("message"));
            //printf("father read end\n!!!");
            write(fd2[1],buf,strlen(buf));
            printf("father recv mesasage: %s\n",buf);
        }
        wait(NULL);
    }
    return 0;
}