#include <stdio.h>
#include <unistd.h>

int main()
{
    char message[20];
    int nums = read(0, message, 20);
    write(1, message, nums);
    printf("first read\n");
    nums = read(5, message, 20);
    printf("nums:%d\n", nums);
    printf("second read\n");
    nums = read(5, message, 20);
    printf("nums:%d\n", nums);
    printf("third read\n");
    nums = read(5, message, 20);
    printf("nums:%d\n", nums);
    printf("fouth read\n");
    nums = read(5, message, 20);
    printf("nums:%d\n", nums);
    printf("end\n");
    printf("before write to not exist file descriptor\n");
    write(5, message, 5);
    printf("end write to not exist file descriptor\n");
    return 0;
}