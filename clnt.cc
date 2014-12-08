#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <sys/select.h>

#define LISTENQ 1
#define MAXLINE 4096
#define MAXSOCKADDR 128
#define BUFFSIZE 8192
#define SERV_PORT 9878
#define SERV_PORT_STR "9878"

int main (int argc, char **argv)
{
    //int i;
    int sockfd;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE];
    char recvline[MAXLINE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    while(fgets(sendline, MAXLINE, stdin) != NULL)
    {
        write(sockfd, sendline, strlen(sendline));

        memset(recvline, 0, MAXLINE);

        if (read(sockfd, recvline, MAXLINE) == 0)
        {
            perror("Server terminated prematurely");
            exit(EXIT_FAILURE);
        }

        fputs(recvline,stdout);
    }

    return 0;
}
