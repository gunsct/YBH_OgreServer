#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <process.h>

#include "Packet.h"
//#include "Parser.h"
#include "Synchronization.h"

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512


class Client{
private:
	WSADATA wsa;
	SOCKET sock;
	SOCKADDR_IN serveraddr;
	Critical Cr;
	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE + 1];
	int len;

	//패킷에 사용할 변수
	int retval;
	int player_num;
	int other_num;
	int fugitive_num, catcher_num;
	bool regi,endflag;
	float x, y, z;

	//Parser Ps;
	clock_t start, finish;//타이머용
	double duration;

public:
	Client();
	~Client();
	packetClientRegi RegiCli;
	packetSetPos Pos;
	packetStart Start;
	packetEnd End;

	int recvn(SOCKET s, char *buf, int len, int flags);

	void set_socket();
	void set_connet();
	void MsgSender(packetHeader* packet);
	int MsgReceiver();
	void transfer();

	void run_client();


	void set_pos(float _x, float _y, float _z);
	void set_flag(bool _flag);
	float get_xp();
	float get_yp();
	float get_zp();
	int get_pn();
	int get_on();
};