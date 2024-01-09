#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    char c = '*';
    char buf[20];
    sleep(5);
    memset(buf, 0, sizeof(buf));
    int rc = read(0, &buf, 19);
    printf("rc:%d buf:%s\n", rc, buf);
    memset(buf, 0, sizeof(buf));
    rc = read(0, &buf, 19);
    printf("rc:%d buf:%s\n", rc, buf);
    memset(buf, 0, sizeof(buf));
    rc = read(0, &buf, 19);
    printf("rc:%d buf:%s\n", rc, buf);
    return 0;
}