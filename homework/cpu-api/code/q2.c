#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(int argc, char *argv[] )
{
    int fd = open("1.txt", O_RDWR | O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);
    int rc = fork();
    if(rc == 0)
    {
        //child process
        write(fd, "12345",6);
        char buf[20];
        int result = read(fd, buf, 20);
        printf("child process read: %s \n",buf);
    }
    else
    {
        //parent process
        write(fd, "123456789",10);
        char buf[20];
        int result = read(fd, buf, 20);
        printf("parent process read: %s \n",buf);    
    }
}