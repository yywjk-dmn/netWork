#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>


#define SERVER_PORT 8080
#define BUFFER_SIZE 26
#define SERVER_IP "172.28.25.146"

int main()
{
    /* 创建套接字 */
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("socket error");
        exit(-1);
    }

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);

    int ret = inet_pton(AF_INET, SERVER_IP, (void *)&serverAddress.sin_addr.s_addr);
    if (ret == -1)
    {
        perror("inet_pton error");
        exit(-1);
    }
    
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    char recvBuffer[BUFFER_SIZE];
    memset(recvBuffer, 0, sizeof(recvBuffer) - 1);

    while (1)
    {
        strncpy(buffer, "dhbcdshGCYDBCbcyd", sizeof(buffer) - 1);
        sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

        recvfrom(sockfd, recvBuffer, sizeof(recvBuffer) - 1, 0, NULL, NULL);
        printf("recvbuffer: %s\n", recvBuffer);


    }


    return 0;
}