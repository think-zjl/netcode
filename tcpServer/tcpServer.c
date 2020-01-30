#include <stdio.h>
#include <string.h>

#ifdef linux
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#else
//#include "stdafx.h"
#include <winsock2.h>
#include <WS2tcpip.h>
#endif

int main(int argc, char* argv[])
{
	//初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}
	//创建套接字
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET){
		printf("socket error !");
		return 0;
	}

	//绑定IP和端口
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(slisten, (LPSOCKADDR)& sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
		return 1;
	}

	//开始监听
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}
	//循环接收数据
	struct sockaddr_in remoteAddr;
	char revData[255];
	while (1)
	{
		printf("waitting for connect...\n");
		int nAddrlen = sizeof(remoteAddr);
		SOCKET sClient = accept(slisten, (SOCKADDR*)& remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}

		printf("recv a connect%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		//接收数据
		int ret = recv(sClient, revData, sizeof(revData)-1, 0);
		if (ret > 0)
		{
			revData[ret] = 0x00;
			printf("%s", revData);
		}

		//发送数据
		const char* sendData = "hello! tcp client\n";
		send(sClient, sendData, strlen(sendData), 0);

		closesocket(sClient);
	}

	closesocket(slisten);
	WSACleanup();
	return 0;
}
#if 0
int main(int argc, char* argv[])
{
	int server_sockfd;//服务器端套接字
	int client_sockfd;//客户端套接字
	int len;
	struct sockaddr_in my_addr;   //服务器网络地址结构体
	struct sockaddr_in remote_addr; //客户端网络地址结构体
	int sin_size;
	char buf[BUFSIZ];  //数据传送的缓冲区
	memset(&my_addr, 0, sizeof(my_addr)); //数据初始化--清零
	my_addr.sin_family = AF_INET; //设置为IP通信
	my_addr.sin_addr.s_addr = INADDR_ANY;//服务器IP地址--允许连接到所有本地地址上
	my_addr.sin_port = htons(8000); //服务器端口号

    /*创建服务器端套接字--IPv4协议，面向连接通信，TCP协议*/
	if ((server_sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return 1;
	}

	        /*将套接字绑定到服务器的网络地址上*/
	if (bind(server_sockfd, (struct sockaddr*) & my_addr, sizeof(struct sockaddr)) < 0)
	{
		perror("bind");
		return 1;
	}

	    /*监听连接请求--监听队列长度为5*/
	listen(server_sockfd, 5);

	sin_size = sizeof(struct sockaddr_in);

	    /*等待客户端连接请求到达*/
	if ((client_sockfd = accept(server_sockfd, (struct sockaddr*) & remote_addr, &sin_size)) < 0)
	{
		perror("accept");
		return 1;
	}
	printf("accept client %s\n", inet_ntoa(remote_addr.sin_addr));
	len = send(client_sockfd, "Welcome to my server\n", 21, 0);//发送欢迎信息

    /*接收客户端的数据并将其发送给客户端--recv返回接收到的字节数，send返回发送的字节数*/
	while ((len = recv(client_sockfd, buf, BUFSIZ, 0)) > 0)
	{
		buf[len] = '\0';
		printf("%s\n", buf);
		if (send(client_sockfd, buf, len, 0) < 0)
		{
			perror("write");
			return 1;
		}
	}
	close(client_sockfd);
	close(server_sockfd);
	return 0;
}
#endif
