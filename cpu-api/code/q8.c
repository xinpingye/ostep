#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[] )
{
    int pipe_fd[2];
    pipe(pipe_fd);
    int rc1 = fork();
    if(rc1 == 0)
    {
        // child1 process
        close(pipe_fd[1]);
        dup2(pipe_fd[0],STDIN_FILENO);
        char recv_msg[30];
        fgets(recv_msg, 30, stdin);
        printf("my recv msg is : %s \n",recv_msg);
    }
    else
    {
        int rc2 = fork();
        if(rc2 == 0)
        {
            char message[] = "child 2 send message to child 1 \n";
            close(pipe_fd[0]);
            dup2(pipe_fd[1],STDOUT_FILENO);
            printf("child 2 send message to child 1 \n");
            //child2 process
        }
        else
        {
            //parent process
            wait(NULL);
            //printf("parent process end!!!\n");
        }
    }
    return 0;
}