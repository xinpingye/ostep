#include <sys/time.h>
#include <stdio.h>

int main()
{
    struct timeval time1,time2;
    gettimeofday(&time1, NULL);
    for(int i = 0;i < 10000; i++)
        gettimeofday(&time2, NULL);
    double result = time2.tv_usec - time1.tv_usec;
    printf("time1.tv_sec:%ld time1.tv_usec:%ld time2.tv_sec:%ld time2.tv_usec:%ld", \
            time1.tv_sec, time1.tv_usec, time2.tv_sec, time2.tv_usec);
    printf("10000 call to gettimeofday cost time:%f ms\n",result);
    return 0;
}