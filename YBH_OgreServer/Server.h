#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

#include "Error.h"
#include "Socket_client.h"

#define SERVERPORT 9000
#define BUFSIZE    512

vector <Socket_Cli> cli_info;//클라 정보 저장용

//클라이언트 리스트가 필요함 클라이언트 정보만있는 클래스랑
class Server : public Error{
private:
	int retval;
	WSADATA wsa;
	SOCKET server_sock;
	SOCKADDR_IN serveraddr;

	int cli_num;

	/*SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;*/
	HANDLE hThread;//스레드는 메시지 전달부분이랑 파싱부분 따로 할지 고려해봐야겠음
	//리스트 추가하고 여기에 클라소켓 정보클래스 다 때려박자

public:
	Server();
	~Server();

	void socket_set();
	void bind_set();
	void listen_set();
	void accept_set();
};