#include "Server.h"
#include <iostream>


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
		sockaddr clntInfo;
		int size = sizeof(clntInfo);
		//Принимаем соединение с клиентом
		while (clntSock = accept(srvsock, &clntInfo, &size)) {

			std::cout << "Connect new client made successfully:" << clntInfo.sa_data << std::endl;
			while (resStart = recv(clntSock, srvBuf, 20, 0))
			{
				if (resStart <= 0)
				{
					std::cout << "Client removed" << std::endl;
					closesocket(clntSock);
					break;
				}
				std::cout << srvBuf << std::endl;
			}
			
		}
	}
}


int main()
{
	Server server(MAKEWORD(2,2), 1089);
	return 0;
}
