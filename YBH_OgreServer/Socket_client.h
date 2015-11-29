#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>

//서바가 소캣 클라들 정보만 따로 벡터로 모아서 다시 뿌릴 때 쓰려고 따로 클래스만듬
class Socket_Cli{
private:
	int num;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;

public:
	Socket_Cli(){}
	~Socket_Cli(){}

	int addrlen;

	void set_num(int _num);
	int get_num();

	void set_sock(SOCKET _CS);
	SOCKET get_sock();

	void set_sockaddr(SOCKADDR_IN _CSA);
	SOCKADDR_IN get_sockaddr();
};