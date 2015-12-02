#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

#include "Socket_client.h"
#include "Parser.h"
#include "Synchronization.h"

#define SERVERPORT 9000
#define BUFSIZE    512

vector <Socket_Cli> cli_info;//클라 정보 저장용
int cli_num = 0;

class Ogre_Server{
private:
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	SOCKET listen_sock;
	SOCKADDR_IN serveraddr;

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;

public:
	Ogre_Server();
	~Ogre_Server();

	void set_socket();
	void set_bind();
	void set_listen();
	void accept_transfer();
};