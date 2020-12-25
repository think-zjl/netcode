#include <stdio.h>
#include <string.h>
#ifdef linux
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") 
#endif

#define UDP_SERVER_IP	"122.112.171.40"
#define UDP_SERVER_PORT	(8000)

#ifdef linux
int main(int argc, char* argv[])
{
	int client_sockfd;
	int len;
	struct sockaddr_in remote_addr;
	int sin_size;
	char buf[BUFSIZ];
	memset(&remote_addr, 0, sizeof(remote_addr));
	remote_addr.sin_family = AF_INET;
	remote_addr.sin_addr.s_addr = inet_addr(argv[1]);
	remote_addr.sin_port = htons(UDP_SERVER_PORT);

	if ((client_sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket");
		return 1;
	}
	strcpy(buf, "This is a test message");
	printf("sending: '%s'\n", buf);
	sin_size = sizeof(struct sockaddr_in);

	if ((len = sendto(client_sockfd, buf, strlen(buf), 0, (struct sockaddr*) & remote_addr, sizeof(struct sockaddr))) < 0){
		perror("recvfrom");
		return 1;
	}
	char recvData[255] = {0};
	int ret = recvfrom(client_sockfd, recvData, 255, 0, (struct sockaddr*)& remote_addr, sizeof(struct sockaddr_in));
	if (ret > 0){
		recvData[ret] = 0x00;
		printf(recvData);
	}
	printf("recvData:%s\n", recvData);
	close(client_sockfd);
	return 0;
}
#else
int main(int argc, char* argv[])
{
	WORD socketVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(socketVersion, &wsaData) != 0){
		return 0;
	}
	SOCKET sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(UDP_SERVER_PORT);
	sin.sin_addr.S_un.S_addr = inet_addr(UDP_SERVER_IP);
	int len = sizeof(sin);

	const char* sendData = "Clinet send\n";
	sendto(sclient, sendData, strlen(sendData), 0, (struct sockaddr*)&sin, len);

	char recvData[255];
	int ret = recvfrom(sclient, recvData, 255, 0, (struct sockaddr*)& sin, &len);
	if (ret > 0){
		recvData[ret] = 0x00;
		printf(recvData);
	}

	closesocket(sclient);
	WSACleanup();
	return 0;
}
#endif
