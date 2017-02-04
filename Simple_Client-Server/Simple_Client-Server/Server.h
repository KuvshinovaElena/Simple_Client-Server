#pragma once
#pragma comment(lib, "Wsock32.lib")
#include <winsock2.h>
#include <vector>
#include <map>

class Server
{
	std::map<USHORT, SOCKET> clients;	//Клиенты
public:
	Server(const WORD wVersionRequest, const int port);
	//int updateClient(char * buff, USHORT port);
	class ExceptionInvalidSocketServer : std::exception {};
	class ExceptionWSAStartup : std::exception {};
	class ExceptionServerBind : std::exception {};
	class ExceptionServerListen : std::exception {};
	class ExceptionServerAccept : std::exception {};
	class ExceptionServerReceive : std::exception {};
};
