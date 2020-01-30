//#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") 

int main(int argc, char* argv[])
{
	WORD socketVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(socketVersion, &wsaData) != 0)
	{
		return 0;
	}
	SOCKET sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int len = sizeof(sin);

	const char* sendData = "���Կͻ��˵����ݰ�.\n";
	sendto(sclient, sendData, strlen(sendData), 0, (sockaddr*)& sin, len);

	char recvData[255];
	int ret = recvfrom(sclient, recvData, 255, 0, (sockaddr*)& sin, &len);
	if (ret > 0)
	{
		recvData[ret] = 0x00;
		printf(recvData);
	}

	closesocket(sclient);
	WSACleanup();
	return 0;
}
#if 0
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
	int client_sockfd;
	int len;
	struct sockaddr_in remote_addr; //�������������ַ�ṹ��
	int sin_size;
	char buf[BUFSIZ];  //���ݴ��͵Ļ�����
	memset(&remote_addr, 0, sizeof(remote_addr)); //���ݳ�ʼ��--����
	remote_addr.sin_family = AF_INET; //����ΪIPͨ��
	remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//������IP��ַ
	remote_addr.sin_port = htons(8000); //�������˿ں�

		/*�����ͻ����׽���--IPv4Э�飬����������ͨ�ţ�UDPЭ��*/
	if ((client_sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket");
		return 1;
	}
	strcpy(buf, "This is a test message");
	printf("sending: '%s'\n", buf);
	sin_size = sizeof(struct sockaddr_in);

	/*��������������ݰ�*/
	if ((len = sendto(client_sockfd, buf, strlen(buf), 0, (struct sockaddr*) & remote_addr, sizeof(struct sockaddr))) < 0)
	{
		perror("recvfrom");
		return 1;
	}
	close(client_sockfd);
	return 0;
}
#endif