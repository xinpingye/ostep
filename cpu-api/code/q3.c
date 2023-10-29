#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[] )
{
    //int x = 100;
    //printf("x:%d\n",x);
    int rc = fork();
    if(rc == 0)
    {
        // child process
        printf("hello  ");
        //x += 100;
        //printf("child process x:%d\n",x);
    }
    else
    {
        sleep(5);
        printf("goodbye\n");
    }
    return 0;
}