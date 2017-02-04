#include "Server.h"
#include <iostream>
#include <process.h>
#include <algorithm>
#define BUFFSIZE 300
struct argList
{
	SOCKET client;
	sockaddr_in clntInfo;
	std::map<USHORT, SOCKET>* clnts;
	argList(SOCKET sock, sockaddr_in inf, std::map<USHORT, SOCKET> mapclnt)
	{
		client = sock;
		clntInfo = inf;
		clnts = &mapclnt;
	}

};

void serviceClient(void * arg);

Server::Server(const WORD wVersionRequest, const int port)
{
	WSAData wsa;		//Объявляем экземпляр структуры, содержащей информацию о реализации Windows Sokets

	int resStart = WSAStartup(wVersionRequest, &wsa);		//Загружаем бииблиотеку WinSocks 2.2, информация будет находиться в wsa
	if (resStart != 0)
	{
		// Ошибка загрузки библитеки
		std::cout << "Error at WSAStartup " << resStart << std::endl;
		//Выбросить исключение. Найти какое
		throw ExceptionWSAStartup();
	}
	SOCKET srvsock = INVALID_SOCKET;
	srvsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	/*
	AF_INET - используется IPv4
	SOCK_STREAM - потоковый сокет (с установкой соединения)
	IPPROTO_TCP - используем TCP протокол
	*/
	if (srvsock == INVALID_SOCKET)
	{
		std::cout << "Error at create socket " << WSAGetLastError() << std::endl;
		WSACleanup();
		throw ExceptionInvalidSocketServer();
	}
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = port;
	//INADDR_ANY - Сервер будет обрабатывать все запросы на указанный порт, независимо от его IP адреса
	sin.sin_addr.s_addr = 0;
	//Связываем сокет с конкретным сетевым адресом
	resStart = bind(srvsock, (SOCKADDR*)&sin, sizeof(sin));
	if (resStart != 0)
	{
		std::cout << "Bind failed with error " << WSAGetLastError() << std::endl;
		closesocket(srvsock);
		WSACleanup();
		throw ExceptionServerBind();
	}
	else
	{
		std::cout << "Successful bind!" << std::endl;
		std::cout << "Port " << sin.sin_port << std::endl << std::endl;;
	}
	//SOMAXCONN - максимальная разумная длина очереди клиентов для текущей системы (Например: 2147483647)
	if (listen(srvsock, SOMAXCONN) != 0)
	{
		std::cout << "Listen failed with error " << WSAGetLastError() << std::endl;
		closesocket(srvsock);
		WSACleanup();
		throw ExceptionServerListen();
	}
	else
	{
		std::cout << "Waiting for clients request" << std::endl;
	}
	while (true)
	{
		SOCKET clntSock = INVALID_SOCKET;
		sockaddr_in clntInfo;
		int size = sizeof(clntInfo);
		//Принимаем соединение с клиентом
		while (clntSock = accept(srvsock, (sockaddr *)&clntInfo, &size)) {
			clients.insert(std::pair<USHORT, SOCKET>(clntInfo.sin_port, clntSock));
			std::cout << "Connect new client made successfully:" << ntohs(clntInfo.sin_port) << std::endl;
			//clients->insert(std::pair<int, SOCKET>(ntohs(clntInfo.sin_port), clntSock));
			argList *arg = new argList(clntSock,clntInfo,clients); 

			_beginthread(serviceClient, 0, (void *)arg);

		}
	}
}
void serviceClient(void * arg)
{
	argList & argClnt = *reinterpret_cast<argList*>(arg);
	int res = 0;
	char srvBuf[BUFFSIZE];
	send(argClnt.client, "Hi, client!", BUFFSIZE, 0);
	while (res = recv(argClnt.client, srvBuf, BUFFSIZE, 0))
	{
		if (res <= 0)
		{
			std::cout << "Client removed" << std::endl;
			closesocket(argClnt.client);
			break;
		}
		for (std::map<USHORT, SOCKET>::const_iterator p = argClnt.clnts->begin(); (p != argClnt.clnts->end()) && (p->first != argClnt.clntInfo.sin_port); ++p)
		{
			//send(p->second, (char*)ntohs(argClnt.clntInfo.sin_port), 10, 0);
			send(p->second, "HI", 10, 0);
			//send(p->second, srvBuf, BUFFSIZE, 0);
		}
		std::cout << "Client " << ntohs(argClnt.clntInfo.sin_port) << ":\t" << srvBuf << std::endl;
	}
	_endthread();
}

int main()
{
	Server server(MAKEWORD(2,2), 1089);
	return 0;
}

//int Server::updateClient(char * buff, USHORT port)
//{
//	for (std::map<int, SOCKET>::const_iterator p = clients->begin(); (p != clients->end()) && (p->first != port); ++p)
//	{
//		send(p->second, (char*)port, 10, 0);
//		send(p->second, buff, BUFFSIZE, 0);
//	}
//	return 0;
//}