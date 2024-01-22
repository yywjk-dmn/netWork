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

int main()
{
    /* 创建新的套接字 */
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("socket error");
        exit(-1);
    }

    /* 本地地址 */
    struct sockaddr_in localAddress;
    /* 清除脏数据 */
    memset(&localAddress, 0, sizeof(localAddress));

    /* 将本地的地址转换成服务器可以识别的状态 地址族 */
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(SERVER_PORT);
    localAddress.sin_addr.s_addr = INADDR_ANY;

    int localAddressLen = sizeof(localAddress);
    /* 绑定 */
    int ret = bind(sockfd, (struct sockaddr *)&localAddress, localAddressLen);
    if (ret == -1)
    {
        perror("bind error");
        exit(-1);
    }

    /* 接收数据的数组 */
    char recvbuffer[BUFFER_SIZE];
    memset(recvbuffer, 0, sizeof(recvbuffer));

    struct sockaddr_in clientAddress;
    memset(&clientAddress, 0, sizeof(clientAddress));

    socklen_t clientAddressLen = sizeof(clientAddress);
    int recvBytes = 0;
    while (1)
    {
        recvBytes = recvfrom(sockfd, recvbuffer, sizeof(recvbuffer), 0, (struct sockaddr *)&clientAddress, &clientAddressLen);
        if (recvBytes <= 0)
        {
            perror("recv error");
            exit(-1);
        }
        else
        {
            for (int idx = 0; idx < recvBytes; idx++)
            {
                if (recvbuffer[idx] >= 'a' && recvbuffer[idx] <= 'z')
                {
                    recvbuffer[idx] -= 32;
                }
            }
            /*将转换过的数据再次返回客户端 */
            sendto(sockfd, recvbuffer, recvBytes, 0, NULL, 0);
        }


    }

    close(sockfd);



    return 0;
}