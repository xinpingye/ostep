#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

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

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port =  htons(atoi(argv[1]));

    bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    printf("serv_addr.sin_addr.s_addr:%#lx \n",serv_addr.sin_addr.s_addr);

    listen(serv_sock,5);

    printf("serv_addr.sin_addr.s_addr:%#lx \n",serv_addr.sin_addr.s_addr);

    clnt_addr_size = sizeof(clnt_addr);

    while(1)
    {

        clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);

        printf("serv_addr.sin_addr.s_addr:%#lx \n",serv_addr.sin_addr.s_addr);

        char * time_str = get_time_myself();

        write(clnt_sock,time_str,strlen(time_str) + 1);

        printf("server send message:%s  and strlen(str) + 1 is :%d\n", time_str, strlen(time_str) + 1);

        close(clnt_sock);

    }

    close(serv_sock);

    return 0;
}