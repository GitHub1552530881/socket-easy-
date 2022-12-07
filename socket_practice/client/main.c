#if 1

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>//sockaddr_in头文件
#include <arpa/inet.h>//ip转换函数
#include<string.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
	int cfd = socket(AF_INET, SOCK_STREAM, 0);
	if (cfd == -1)
	{
		perror("socket");
		exit(0);
	}

	struct sockaddr_in addr;
	//addr.sin_addr.s_addr = inet_addr("192.168.79.129");
	addr.sin_port = htons(5000);
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, "192.168.79.129", &addr.sin_addr.s_addr);

	int ret = connect(cfd, (struct sockaddr*)&addr, sizeof(addr));
	if (ret == -1)
	{
		perror("connect");
		exit(0);
	}

	
	//char data[1024];
	int number = 0;
	while (1)
	{
		/*memset(data, 0, sizeof(data));
		scanf("%s", &data);
		int sendLen = send(cfd, data, sizeof(data), 0);
		if (sendLen == -1)
		{
			perror("send");
			exit(0);
		}*/
		char buf[1024];
		sprintf(buf, "你好, 服务器...%d", number++);
		send(cfd, buf, strlen(buf) + 1, 0);

		memset(buf, 0, sizeof(buf));
		int len = recv(cfd, buf, sizeof(buf), 0);
		if (len > 0)
		{
			printf("server say:%s\n", buf);
		}
		else if (len == 0)
		{
			printf("server leave\n");
			break;
		}
		else
		{
			printf("recv\n");
			break;
		}
		sleep(1);
	}

	close(cfd);
    return 0;
}

#else
// client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
	// 1. 创建通信的套接字
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		perror("socket");
		exit(0);
	}

	// 2. 连接服务器
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);   // 大端端口
	inet_pton(AF_INET, "192.168.79.129", &addr.sin_addr.s_addr);

	int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
	if (ret == -1)
	{
		perror("connect");
		exit(0);
	}

	// 3. 和服务器端通信
	int number = 0;
	while (1)
	{
		// 发送数据
		char buf[1024];
		sprintf(buf, "你好, 服务器...%d\n", number++);
		write(fd, buf, strlen(buf) + 1);

		// 接收数据
		memset(buf, 0, sizeof(buf));
		int len = read(fd, buf, sizeof(buf));
		if (len > 0)
		{
			printf("服务器say: %s\n", buf);
		}
		else if (len == 0)
		{
			printf("服务器断开了连接...\n");
			break;
		}
		else
		{
			perror("read");
			break;
		}
		sleep(1);   // 每隔1s发送一条数据
	}

	close(fd);

	return 0;
}
#endif