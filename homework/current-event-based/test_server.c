#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>

int main(int argc,char * argv[])
{

    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    if(argc!=2)
    {
        printf(":Usage : %s <port>\n",argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    printf("serv_sock:%d\n", serv_sock);
    //fcntl(serv_sock, F_SETFL, fcntl(serv_sock, F_GETFL, 0) | O_NONBLOCK);

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port =  htons(atoi(argv[1]));

    bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    printf("serv_addr.sin_addr.s_addr:%#lx \n",serv_addr.sin_addr.s_addr);
    listen(serv_sock,5);

    clnt_addr_size = sizeof(clnt_addr);

    while(1)
    {
        int rc = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
        if (rc != -1)
        {
            clnt_sock = rc;
            char message[20] = "return string";
            write(clnt_sock, message, strlen(message));
            memset(message, 0, sizeof(message));
            int nums = read(clnt_sock, message, sizeof(message));
            write(1, message, nums);
            write(clnt_sock, message, nums);
            memset(message, 0, sizeof(message));
            printf("before first read\n");
            nums = read(clnt_sock, message, sizeof(message));
            printf("read nums is %d\n", nums);
            write(1, message, nums);
            memset(message, 0, sizeof(message));
            printf("before second read\n");
            nums = read(clnt_sock, message, sizeof(message));
            printf("read nums is %d\n", nums);
            write(1, message, nums);
            memset(message, 0, sizeof(message));
            printf("before third read\n");
            nums = read(clnt_sock, message, sizeof(message));
            printf("read nums is %d\n", nums);
            write(1, message, nums);
            memset(message, 0, sizeof(message));
            printf("before four read\n");
            nums = read(clnt_sock, message, sizeof(message));
            printf("read nums is %d\n", nums);
            write(1, message, nums);
            printf("end\n");
            int num = 0;
            while(1)
            {
                memset(message, 0, sizeof(message));
                printf("before %d read\n", num++);
                if (num == 10)
                {
                    printf("before try to write to socket\n");
                    write(clnt_sock, message, 2);
                    printf("after try to write to socket\n");
                }
                if (num == 15)
                    break;
                nums = read(clnt_sock, message, sizeof(message));
                printf("read nums is %d\n", nums);
                write(1, message, nums);
            }
        }
    }
    close(serv_sock);
    return 0;
}