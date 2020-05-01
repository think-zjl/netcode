#include <stdio.h>
#include <string.h>
#ifdef linux
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#else
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") 
#endif

#define UDP_SERVER_PORT	(8000)

#ifdef linux
int main(int argc, char* argv[])
{
	int server_sockfd;
	int len;
	struct sockaddr_in my_addr;//服务器网络地址结构体
	struct sockaddr_in remote_addr;//客户端网络地址结构体
	int sin_size;
	char buf[BUFSIZ];//数据传送的缓冲区
	memset(&my_addr, 0, sizeof(my_addr));//数据初始化--清零
	my_addr.sin_family = AF_INET;//设置为IP通信
	my_addr.sin_addr.s_addr = INADDR_ANY;//服务器IP地址--允许连接到所有本地地址上
	my_addr.sin_port = htons(UDP_SERVER_PORT);//服务器端口号
	printf("%d\r\n", IPPROTO_UDP);
	/*创建服务器端套接字--IPv4协议，面向无连接通信，UDP协议*/
	if ((server_sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket");
		return 1;
	}

	/*将套接字绑定到服务器的网络地址上*/
	if (bind(server_sockfd, (struct sockaddr*) & my_addr, sizeof(struct sockaddr)) < 0){
		perror("bind");
		return 1;
	}
	sin_size = sizeof(struct sockaddr_in);
	printf("waiting for a packet...\n");

	while(1){
		/*接收客户端的数据并将其发送给客户端--recvfrom是无连接的*/
		if ((len = recvfrom(server_sockfd, buf, BUFSIZ, 0, (struct sockaddr*) & remote_addr, &sin_size)) < 0){
			perror("recvfrom");
			return 1;
		}
		printf("received packet from %s: port:%d\n", inet_ntoa(remote_addr.sin_addr), remote_addr.sin_port);
		buf[len] = '\0';
		printf("contents: %s\n", buf);
		char *snd_data = inet_ntoa(remote_addr.sin_addr);
		if ((len = sendto(server_sockfd, snd_data, strlen(snd_data), 0, (struct sockaddr*)&remote_addr, sizeof(struct sockaddr))) < 0){
			perror("sendto");
			return 1;
		}
	}
	
	close(server_sockfd);
	return 0;
}
#else
int main(int argc, char* argv[])
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &wsaData) != 0){
		return 0;
	}

	SOCKET serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (serSocket == INVALID_SOCKET){
		printf("socket error !");
		return 0;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(UDP_SERVER_PORT);
	serAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(serSocket, (sockaddr*)& serAddr, sizeof(serAddr)) == SOCKET_ERROR){
		printf("bind error !");
		closesocket(serSocket);
		return 0;
	}

	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(remoteAddr);
	while (true){
		char recvData[255];
		int ret = recvfrom(serSocket, recvData, 255, 0, (sockaddr*)& remoteAddr, &nAddrLen);
		if (ret > 0){
			recvData[ret] = 0x00;
			printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
			printf(recvData);
		}

		const char* sendData = "一个来自服务端的UDP数据包\n";
		sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr*)& remoteAddr, nAddrLen);
	}
	closesocket(serSocket);
	WSACleanup();
	return 0;
}
#endif
