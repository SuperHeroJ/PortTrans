// PortTrans.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Winsock2.h>
#include <cstdio>
#pragma comment(lib,"ws2_32.lib")

void main()
{
	WSADATA wsaData;
	SOCKET sockServer;
	SOCKADDR_IN addrServer;
	SOCKET sockClient;
	SOCKADDR_IN addrClient;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sockServer = socket(AF_INET, SOCK_STREAM, 0);
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//INADDR_ANY表示任何IP
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(6000);//绑定端口6000
	bind(sockServer, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));

	//Listen监听端
	listen(sockServer, 5);//5为等待连接数目
	printf("中转服务器已启动:\n传输中...\n");
	int len = sizeof(SOCKADDR);
	//char sendBufS[100];//发送至客户端的字符串
	char recvBufS[100];//接受客户端返回的字符串
	 //会阻塞进程，直到有客户端连接上来为止

	//------------------------------------------------------------------------------------------
	SOCKET sockHost;//客户端Socket
	SOCKADDR_IN addrServer2;//服务端地址
	//char sendBufC[100];//发送至服务器的字符串
	char recvBufC[100];//接受服务器返回的字符串
	//新建客户端socket
	sockHost = socket(AF_INET, SOCK_STREAM, 0);
	//定义要连接的服务端地址
	addrServer2.sin_addr.S_un.S_addr = inet_addr("172.16.1.115");//目标IP(127.0.0.1是回送地址)
	addrServer2.sin_family = AF_INET;
	addrServer2.sin_port = htons(7000);//连接端口6000
									  //连接到服务端
	connect(sockHost, (SOCKADDR*)&addrServer2, sizeof(SOCKADDR));
	//------------------------------------------------------------------------------------------
	sockClient = accept(sockServer, (SOCKADDR*)&addrClient, &len);
	//接收并打印客户端数据
	while(1)
	{
		ZeroMemory(recvBufS, 100);
		recv(sockClient, recvBufS, 100, 0);
		printf("中转：客户端->服务器:%s\n", recvBufS);
		send(sockHost, recvBufS, strlen(recvBufS) + 1, 0);
		ZeroMemory(recvBufC, 100);
		recv(sockHost, recvBufC, 100, 0);
		printf("中转：服务器->客户端:%s\n", recvBufC);
		send(sockClient, recvBufC, strlen(recvBufC) + 1, 0);
	}
	closesocket(sockServer);
	closesocket(sockClient);
	closesocket(sockHost);
	WSACleanup();
}

