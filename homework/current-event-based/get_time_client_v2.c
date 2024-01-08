#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


int fread_form_stdin_line_by_line(char * a)
{
    int idex = 0;
    while(1)
    {
        fread(&(a[idex]), 1, 1, stdin);
        if (a[idex++] == '\n')
            return idex;
    }
}
int main(int argc,char * argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    int str_len = 0;

    if(argc!=3)
    {
        printf("Usage : %s <IP> <port>\n",argv[0]);
        exit(0);
    }

    sock = socket(PF_INET,SOCK_STREAM,0);

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));

    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    char message[100];
    int nums = read(sock, message, sizeof(message));
    write(1, message, nums);

    while(1)
    {
        char a[100];
        memset(a, 0, sizeof(a));
        int nums = fread_form_stdin_line_by_line(a);
        write(sock, a, nums);
        memset(a, 0, sizeof(a));
        nums = read(sock, a, sizeof(a));
        write(1, a, nums);
    }
    close(sock);
    return 0;
}