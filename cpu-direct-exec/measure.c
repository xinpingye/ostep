#include <stdio.h>
#include <sys/time.h>
#include <x86intrin.h>

#pragma intrinsic(__rdtsc)
int main()
{
    struct timeval val1,val2;
    //int ret1 = gettimeofday(&val1, NULL);
    //int ret2 = gettimeofday(&val2, NULL);
    unsigned long int  ret1 = __rdtsc();
    gettimeofday(&val1, NULL);
    unsigned long int ret2 = __rdtsc();
    printf("ret1:%ld ret2:%ld\n",ret1,ret2);
    //if(ret1 == 0 && ret2 == 0)
    //{
    //    int ms = val2.tv_usec - val1.tv_usec;
    //    printf("one gettimeofday consume %d ms\n",ms);
    //    return 0;
    //}
    
    return 0;
}