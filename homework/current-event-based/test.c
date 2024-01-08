#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

void func()
{
	char a[10];
    memset(a, 0, sizeof(a));
    int nums = 0;
    while((nums = fread(a, 1, sizeof(a), stdin)) == sizeof(a))
    {
        printf("nums:%d\n", nums);
        fwrite(a, 1, nums, stdout);
    }
    fwrite(a, 1, nums, stdout);
	return;
}

void func2()
{
    char a[10];
    memset(a, 0, sizeof(a));
    int nums = 0;
    while((nums = read(0, a, sizeof(a))) != 0)
    {
        printf("nums:%d\n", nums);
        write(1, a, nums);
    }
    write(1, a, nums);
	return;
}

int main()
{
    /*
    func2();
    printf("next test\n");
    func();
    printf("end");
    */
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    fd_set readFDs;
    while(1)
    {
        select(1, &readFDs, NULL, NULL, &timeout);
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        printf("loop\n");
    }

    return 0;
}