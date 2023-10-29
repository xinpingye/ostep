#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[] )
{
    int rc = fork();
    if(rc == 0)
    {
        // child process
        //wait(NULL);
        printf("child process begin\n");
        char *myargs[3];
        myargs[0] = strdup("ls");
        myargs[1] = strdup("./");
        myargs[2] = NULL;
        execvp(myargs[0],myargs);
        printf("this should be prnt\n");
    }
    else
    {
        int rc_wait = wait(NULL);
        printf("child pid:%d parent process and rc_wait:%d", rc, rc_wait);
        printf("end\n");
    }
    return 0;
}