#ifdef linux
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#endif
#ifdef linux
int main(int argc, char* argv[])
{
	int client_sockfd;
	int len;
	struct sockaddr_in remote_addr;//服务器端网络地址结构体
	char buf[BUFSIZ] = "hell0 tcpServer\r\n";//数据传送的缓冲区
	memset(&remote_addr, 0, sizeof(remote_addr));//数据初始化--清零
	remote_addr.sin_family = AF_INET;//设置为IP通信
	remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//服务器IP地址
	remote_addr.sin_port = htons(8000);//服务器端口号

	/*创建客户端套接字--IPv4协议，面向连接通信，TCP协议*/
	if ((client_sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return 1;
	}

	/*将套接字绑定到服务器的网络地址上*/
	if (connect(client_sockfd, (struct sockaddr*) & remote_addr, sizeof(struct sockaddr)) < 0)
	{
		perror("connect");
		return 1;
	}
	printf("connected to server\n");
	//len = recv(client_sockfd, buf, BUFSIZ, 0);//接收服务器端信息
	//buf[len] = '\0';
	//printf("%s", buf);//打印服务器端信息

	/*循环的发送接收信息并打印接收信息--recv返回接收到的字节数，send返回发送的字节数*/
	while (1)
	{
		//printf("Enter string to send:");
		//scanf("%s", buf);
		//if (!strcmp(buf, "quit"))
		//	break;
		len = send(client_sockfd, buf, strlen(buf), 0);
		len = recv(client_sockfd, buf, BUFSIZ, 0);
		buf[len] = '\0';
		printf("received:%s\n", buf);
	}
	close(client_sockfd);//关闭套接字
	return 0;
}
#else
int main(int argc, char* argv[])
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}

	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		printf("invalid socket !");
		return 0;
	}

	struct sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8888);
	serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//inet_pton(AF_INET, "127.0.0.1", &serAddr.sin_addr.S_un.S_addr);
	if (connect(sclient, (struct sockaddr*)& serAddr, sizeof(struct sockaddr)) == SOCKET_ERROR)
	{
		printf("connect error !");
		closesocket(sclient);
		return 0;
	}
	const char* sendData = "你好，TCP服务端，我是客户端!\n";
	send(sclient, sendData, strlen(sendData), 0);

	char recData[256];
	int ret = recv(sclient, recData, 255, 0);
	if (ret > 0)
	{
		recData[ret] = 0x00;
		printf(recData);
	}
	closesocket(sclient);
	WSACleanup();
	return 0;
}
#endif

