#pragma once
#include "Book.h"
#pragma comment(lib, "Wsock32.lib")
#include <winsock2.h>
#include <vector>
#include <map>

class Server
{
	std::vector<Book> books;	//Данные о книгах
	std::map<int, SOCKET> clients;	//Клиенты
	char srvBuf[20];	//Серверный буфер
public:
	Server(const WORD wVersionRequest, const int port);
	
	std::vector<Book> getData() const { return books; }
	class ExceptionInvalidSocketServer : std::exception {};
	class ExceptionWSAStartup : std::exception {};
	class ExceptionServerBind : std::exception {};
	class ExceptionServerListen : std::exception {};
	class ExceptionServerAccept : std::exception {};
	class ExceptionServerReceive : std::exception {};
};