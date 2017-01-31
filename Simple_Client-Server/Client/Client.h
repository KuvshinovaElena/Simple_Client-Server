#pragma once
#pragma comment(lib, "Wsock32.lib")
#include <winsock2.h>
#include <iostream>

class Client
{
	char clntBuf[20];
public:
	Client(const WORD wVersionRequest, const int port);

	class ExceptionInvalidSocketClient : std::exception {};
	class ExceptionClientConnect : std::exception {};
	class ExceptionClientSendData : std::exception {};
	class ExceptionWSAStartup : std::exception {};
};
