#pragma once
#include <winsock2.h> 
#pragma comment(lib, "ws2_32.lib")
#include "Packet.h"

#define BUFSIZE 512

class NetworkManager
{
private:

	SOCKET m_client_sock;
	SOCKADDR_IN m_clientaddr;
	char m_cBuf[BUFSIZE+1];
	int m_iError;

	bool m_bOtherPlayer;

public:
	NetworkManager(void);
	~NetworkManager(void);

	bool Init();
	void CleanUp();

	static NetworkManager* GetThis();

	bool Connect(char* ip_addr = "127.0.0.1", int port = 9000);

	bool MsgSender( packetHeader* packet );
	int MsgReceiver();
	int recvn( SOCKET s, char*buf, int len, int flags );

	bool WaitingPlayer();
};

#ifndef NETWORK
#define NETWORK NetworkManager::GetThis()
#endif

