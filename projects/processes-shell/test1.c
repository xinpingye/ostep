#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main()
{
    char* argv[3] ;
    argv[0] = strdup("/bin/ls");
    argv[1] = strdup("");
    argv[2] = NULL;
    execvp(argv[0], argv);
    return 0;
}