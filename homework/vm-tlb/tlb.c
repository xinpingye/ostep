#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("two arguments need\n");
        exit(1);
    }
    int page_nums = atoi(argv[1]);
    assert(1 <= page_nums && page_nums <= 40);
    int test_nums = atoi(argv[2]);
    assert(1 <= test_nums && test_nums <= 10);
    int array[1024 * 40];
    int jump  = 1024;
    struct timeval time1,time2;
    gettimeofday(&time1, NULL);
    for (int i = 0; i < 100 * 10000 * test_nums; i++)
    {
        for (int j = 0;j < page_nums * jump; j += jump)
        array[j] += 1;
    }
    gettimeofday(&time2, NULL);
    /*
    printf("time1.tv_sec:%ld time1.tv_usec:%ld time2.tv_sec:%ld time2.tv_usec:%ld \n", \
            time1.tv_sec, time1.tv_usec, time2.tv_sec, time2.tv_usec);
    */
    if (time1.tv_sec == time2.tv_sec)
        //printf("(time2.tv_usec - time1.tv_usec)/(double)(test_nums * page_nums):%f\n", (time2.tv_usec - time1.tv_usec)/(double)(test_nums * page_nums));
        printf("%f\n", (time2.tv_usec - time1.tv_usec)/(double)(test_nums * page_nums));
    else
        //printf("(time2.tv_usec + (time2.tv_sec - time1.tv_sec) * 1000000 - time1.tv_usec)/(double)(test_nums * page_nums):%f\n", (time2.tv_usec + (time2.tv_sec - time1.tv_sec) * 1000000  - time1.tv_usec)/(double)(test_nums * page_nums));
        printf("%f\n", (time2.tv_usec + (time2.tv_sec - time1.tv_sec) * 1000000  - time1.tv_usec)/(double)(test_nums * page_nums));
    return 0;
}