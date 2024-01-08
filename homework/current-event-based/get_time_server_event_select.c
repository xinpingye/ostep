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

//void error_handing(char *message);

char* get_time_myself()
{
	time_t timep;
	time(&timep);
	char *s = ctime(&timep);
    return s;
}

int main(int argc,char * argv[])
{
    struct sockaddr_in array_storge_client[30];

    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[]="123";

    if(argc!=2)
    {
        printf(":Usage : %s <port>\n",argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    printf("serv_sock:%d\n", serv_sock);
    fcntl(serv_sock, F_SETFL, fcntl(serv_sock, F_GETFL, 0) | O_NONBLOCK);

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port =  htons(atoi(argv[1]));

    bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    printf("serv_addr.sin_addr.s_addr:%#lx \n",serv_addr.sin_addr.s_addr);
    listen(serv_sock,5);

    clnt_addr_size = sizeof(clnt_addr);

    fd_set readFDs, readFDs_bk;
    FD_ZERO(&readFDs);
    FD_ZERO(&readFDs_bk);
    int min_clnt_sock = 9999;
    int max_clnt_sock = 0;
    struct timeval timeout;
    timeout.tv_usec = 0;
    timeout.tv_sec = 5;
    while(1)
    {
        printf("here4\n");
        readFDs = readFDs_bk;
        int rc = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
        if (rc != -1)
        {
            clnt_sock = rc;
            printf("clnt_sock:%d clnt_addr.sin_addr.s_addr:%#lx \n",clnt_sock,clnt_addr.sin_addr.s_addr);
            char * time_str = get_time_myself();
            write(clnt_sock,time_str,strlen(time_str) + 1);
            printf("server send message:%s  and strlen(str) + 1 is :%d\n", time_str, strlen(time_str) + 1);
            array_storge_client[clnt_sock] = clnt_addr;
            FD_SET(clnt_sock, &readFDs);
            readFDs_bk = readFDs;
            if (min_clnt_sock == 9999)
                min_clnt_sock = clnt_sock;
            max_clnt_sock = clnt_sock;
            memset(&clnt_addr,0,sizeof(clnt_addr));
        }
        printf("here\n");
        rc = select(max_clnt_sock + 1, &readFDs, NULL, NULL, &timeout);
        printf("rc:%d\n",rc);
        timeout.tv_usec = 0;
        timeout.tv_sec = 5;
        printf("here2\n");
        if (rc == 0)
            continue;/*in this case readD=FDs no change*/
        for (int i = min_clnt_sock; i < max_clnt_sock + 1; i++)
        {
            if (FD_ISSET(i, &readFDs))
            {
                printf("sock:%d  begin respose\n", i);
                //read all message  to avoid next select same action happen
                char message[100];
                memset(message, 0, sizeof(message))
                printf("12345\n");
                int nums = read(i, message, sizeof(message));
                printf("1234\n");
                message[nums + 1] = '\0';
                printf("receive message:%s\n", message);
                char* retuen_msg = get_time_myself();
                write(i, retuen_msg, strlen(retuen_msg));
            }
        }
        printf("what happen\n");
        readFDs = readFDs_bk;
        printf("here3\n");
    }

    close(serv_sock);

    return 0;
}