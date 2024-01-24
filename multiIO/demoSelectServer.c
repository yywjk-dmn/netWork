#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <ctype.h>

#define SERVER_PORT 8080
#define MAX_LISTEN 128  //最大监听数
#define BUFFER_SIZE 128

/* 用单进程或者单线程实现并发 */
int main()
{
    /* 创建套接字 流式协议 */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket error");
        exit(-1);
    }

    /* 将本地的IP和端口号绑定 */
    struct sockaddr_in localAddress;
    bzero((void *)&localAddress, sizeof(localAddress));
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(SERVER_PORT);
    localAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    socklen_t localAddressLen = sizeof(localAddress);
    int ret = bind(sockfd, (struct sockaddr *)&localAddress, localAddressLen);
    if (ret == -1)
    {
        perror("bind error");
        exit(-1);
    }

    /* 监听 */
    ret = listen(sockfd, MAX_LISTEN);
    if (ret == -1)
    {
        perror("listen error");
        exit(-1);
    }

    /* 把监听的文件描述符添加到读集合中 让内核帮忙检测 */
    fd_set readset;
    /* 清空集合 */
    FD_ZERO(&readset);
    /* 把监听的文件放到内核里面 让内核帮忙检测 */
    FD_SET(sockfd, &readset);

    int maxfd = sockfd;
    while (1)
    {
        ret = select(maxfd + 1, &readset, NULL, NULL, NULL);
        if (ret = -1)
        {
            perror("select error");
            break;
        }
    }

    /* 如果sockfd在readset集合中 那就说明一定有人要连接 */
    if (FD_ISSET(sockfd, &readset))
    {
        int acceptfd = accept(sockfd, NULL, NULL);
        if (acceptfd == -1)
        {
            perror("accept error");
            break;
        }
        /* 将通信的句柄放在读集合 */
        FD_SET(acceptfd, &readset);

        /* 更新maxfd的值 */
        maxfd = maxfd < acceptfd ? accept : maxfd;

    }

    /* 程序到这个地方 说明可能有通信 */
    for (int idx = 0; idx <= maxfd; idx ++)
    {
        if (idx != sockfd && FD_ISSET(idx, &readset))
        {
            char buffer[BUFFER_SIZE];
            bzero(buffer, sizeof(buffer));
            /* 程序到这里 一定有通信 */
            int readBytes = read(idx, buffer, sizeof(buffer) - 1);
            if (readBytes < 0)
            {
                perro("read error");
                /* 将该通信句柄从监听的读集合中删掉 */
                FD_CLR(idx, &readset);
                /* 关闭文件句柄 */
                close(idx);
                continue;
            }
            else if (readBytes == 0)
            {
                /* 说明客户端断开连接 */
                printf("客户端断开连接。。。。\n");
                /* 将该通信句柄从监听的读集合中删掉 */
                FD_CLR(idx, &readset);
                /* 关闭通信句柄 */
                close(idx);
                continue;
            }
            else
            {
                printf("recv:%s\n", buffer);
                for (int jdx = 0; jdx < readBytes; jdx++)
                {
                    buffer[jdx] = toupper(buffer[jdx]);
                }
                /* 发回客户端 */
                write(idx, buffer, readBytes);
                usleep(500);
            }

        }
    }
    /* 关闭文件描述符 */
    close(sockfd);
    






    return 0;
}