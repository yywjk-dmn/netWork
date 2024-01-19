#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include "threadPool.h"

#define SERVER_PORT 8080
#define MAX_LISTEN 128
#define LOCAL_IPADDRESS "127.0.0.1"
#define BUFFER_SIZE 128

#define MINTHREADS 5
#define MAXTHREADS 10
#define MAXQUEUESIZE 128

/* 线程处理函数 */
void * threadHandle(void *arg)
{
    /* 设置线程分离 */
    pthread_detach(pthread_self());
    /* 通信句柄 */
    int acceptfd = *(int *)arg;

    /* 通信 */

    /* 接收缓冲区 */
    char recvbuffer[BUFFER_SIZE];
    memset(recvbuffer, 0, sizeof(recvbuffer));

    /* 发送缓冲区 */
    char sendBuffer[BUFFER_SIZE];
    memset(sendBuffer, 0, sizeof(sendBuffer));
    /* 读取到的字节数 */
    int readBytes = 0;
    while (1)
    {
        readBytes = read(acceptfd, recvbuffer, sizeof(recvbuffer));
        if (readBytes <= 0)
        {
            perror("read error");
            close(acceptfd);
            break;
        }
        else
        {
            /* 读到的字符串 */
            printf("buffer : %s\n", recvbuffer);

            if (strncmp(recvbuffer, "123456", strlen("123456")) == 0)
            {
                strncpy(sendBuffer, "一起加油123456", sizeof(sendBuffer) - 1);
                sleep(1);
                write(acceptfd, sendBuffer, sizeof(sendBuffer));
            }
            else if (strncmp(recvbuffer, "778899", strlen("778899")) == 0)
            {
                strncpy(sendBuffer, "一起加油778899", sizeof(sendBuffer) - 1);
                sleep(1);
                write(acceptfd, sendBuffer, sizeof(sendBuffer));
            }
        }
    }
    pthread_exit(NULL);

}

int main()
{
    /* 初始化线程池 */
    threadpool_t pool;
    threadPoolInit(&pool, MINTHREADS, MAXTHREADS, MAXQUEUESIZE);

    /* 捕捉信号 */
    //signal(SIGINT, sigHander);
    /* todo..... */
    /* 创建新的套接字 */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket error");
        exit(-1);
    }

    /* 设置端口复用 */
    int enableOpt = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *)&enableOpt, sizeof(enableOpt));
    if (ret == -1)
    {
        perror("setsockopt error");
        exit(-1);

    }

    /* 绑定 */
    struct sockaddr_in localAddress;
    /* 清除脏数据 */
    memset(&localAddress, 0, sizeof(localAddress));

    /* 地址族,端口大小是1-65535(短整型 short) */
    localAddress.sin_family = AF_INET;
    /* 端口需要转成大端 */
    localAddress.sin_port = htons(SERVER_PORT);
#if 1
    /* IP地址转成大端 全0 */
    /* INADDR_ANY = 0x00000000 */
    localAddress.sin_addr.s_addr = INADDR_ANY;
#else
    /* 用这个函数代替 */
    inet_pton(AF_INET, LOCAL_IPADDRESS, &(localAddress.sin_addr.s_addr));
#endif
    int localAddressLen = sizeof(localAddress);
    ret = bind(sockfd, (struct sockaddr *)&localAddress, localAddressLen);
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

    /* 客户的信息 */
    struct sockaddr_in clientAddress;
    memset(&clientAddress, 0, sizeof(clientAddress));

    while (1)
    {
        socklen_t clientAddressLen = 0;
        int acceptfd = accept(sockfd, (struct sockaddr *)&clientAddress, &clientAddressLen);
        if (acceptfd == -1)
        {
            perror("accept error");
            exit(-1);
        }

#if 0
        /* 这种情况会一直创建新的线程 消耗资源 */
        pthread_t tid;
        /* 开一个线程去服务 */
        int ret = pthread_create(&tid, NULL, threadHandle, (void *)&acceptfd);
        if (ret != 0)
        {
            perror("create error");
            exit(-1);
        }
#else
        /* 将任务添加到任务队列 */
        threadPoolAddTask(&pool, threadHandle, (void *)&acceptfd);
#endif
    }

    /* 释放线程池 */
    threadPoolDestory(&pool);

    close(sockfd);
    return 0;
}