#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>

//���ٰ� ��Ĺ Ŭ��� ������ ���� ���ͷ� ��Ƽ� �ٽ� �Ѹ� �� ������ ���� Ŭ��������
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