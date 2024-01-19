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
#define MAX_LISTEN 128
#define LOCAL_IPADDRESS "127.0.0.1"
#define BUFFER_SIZE 128

void sigHander(int sigNum)
{

}
int main()
{
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

    /* 客户的信息 */
    struct sockaddr_in clientAddress;
    memset(&clientAddress, 0, sizeof(clientAddress));

    socklen_t clientAddressLen = 0;
    int acceptfd = accept(sockfd, (struct sockaddr *)&clientAddress, &clientAddressLen);
    if (acceptfd == -1)
    {
        perror("accept error");
        exit(-1);
    }

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    char replayBuffer[BUFFER_SIZE];
    memset(replayBuffer, 0, sizeof(replayBuffer));

    int readBytes = 0;
    while (1)
    {
        readBytes = read(acceptfd, buffer, sizeof(buffer));
        if (readBytes < 0)
        {
            perror("read error");
            printf("111");
            exit(-1);
        }
        else if (readBytes == 0)
        {
            printf("222");
            sleep(2);
            close(acceptfd);
            exit(0);
            
        }
        else
        {
            /* 读到的字符串 */
            printf("buffer : %s\n", buffer);
            sleep(3);

            strncpy(replayBuffer, "加油", sizeof(replayBuffer) - 1);
            write(acceptfd, replayBuffer, sizeof(replayBuffer));
            // if (buffer == "注册")
            // {

            // }
            // else if (buffer == "登陆")
            // {
            //     sleep(3);


            //     write(accept, "666", strlen("666") + 1);

            // }

        }
    }
    close(sockfd);





    return 0;
}