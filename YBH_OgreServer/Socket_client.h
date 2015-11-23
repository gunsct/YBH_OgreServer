#pragma comment(lib, "ws2_32")
#include <winsock2.h>

class Socket_Cli{
private:
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;

public:
	int addrlen;

	void set_sock(SOCKET _CS);
	SOCKET get_sock();

	void set_sockaddr(SOCKADDR_IN _CSA);
	SOCKADDR_IN get_sockaddr();
};