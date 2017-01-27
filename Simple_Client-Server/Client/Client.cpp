#include "Client.h"
#include <iostream>

Client::Client(const WORD wVersionRequest, const int port)
{
	WSAData wsa;		//Объявляем экземпляр структуры, содержащей информацию о реализации Windows Sokets

	int res= WSAStartup(wVersionRequest, &wsa);		//Загружаем бииблиотеку WinSocks 2.2, информация будет находиться в wsa
	if (res != 0)
	{
		// Ошибка загрузки библитеки
		std::cout << "Error at WSAStartup " << res << std::endl;
		//Выбросить исключение. Найти какое
		throw ExceptionWSAStartup();
	}
	SOCKET clntSock = INVALID_SOCKET;
	clntSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	/*
	AF_INET - используется IPv4
	SOCK_STREAM - потоковый сокет (с установкой соединения)
	IPPROTO_TCP - используем TCP протокол
	*/
	if (clntSock == INVALID_SOCKET)
	{
		std::cout << "Error at create socket " << WSAGetLastError() << std::endl;
		WSACleanup();
		throw ExceptionInvalidSocketClient();
	}
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = port;
	sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	res = connect(clntSock, (SOCKADDR*)&sin, sizeof(sin));
	if (res != 0)
	{
		std::cout << "Error connect with server " << WSAGetLastError() << std::endl;
		closesocket(clntSock);
		WSACleanup();
		throw ExceptionClientConnect();
	}
	else
		std::cout << "Successful connected with server"  << std::endl;
	char * data = new char[20];
	
	while (std::cin.getline(data, 20))
	{
		if (!strcmp(data, "exit")) clntSock = -1;
		if (res = send(clntSock, data, 20, 0) <= 0 ) break;

	}
	if (clntSock != -1) closesocket(clntSock);
	WSACleanup();
}


int main()
{
	Client client(MAKEWORD(2,2), 1089);
	system("pause");
	return 0;
}