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
        //printf("wait(NULL):%d \n",wait(NULL));
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
        int rc_wait = waitpid(rc,NULL,0);
        printf("child pid:%d parent process and rc_wait:%d \n", rc, rc_wait);
        printf("end\n");
    }
    return 0;
}