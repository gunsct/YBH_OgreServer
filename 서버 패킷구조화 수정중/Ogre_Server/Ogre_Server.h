#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <process.h>
using namespace std;

#include "Packet.h"
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
	char buf[512];

	int cli_num;
	bool regi;

public:
	Ogre_Server();
	~Ogre_Server();

	packetHeader packetheader;
	packetClientRegi RegiCli;
	packetEnd End;
	packetStart Start;
	packetSetPos Pos;

	
	static Ogre_Server* GetThis();

	void MsgSender(packetHeader* packet, SOCKET _client_sock);
	int MsgReceiver(Socket_Cli _sc);

	void set_socket();
	void set_bind();
	void set_listen();
	void accept_transfer();
};