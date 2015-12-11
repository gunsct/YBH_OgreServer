#include "NetworkManager.h"
#include "MsgReceiver.h"
#include <iostream>
#include <process.h> 


NetworkManager::NetworkManager(void)
{
	Init();
}

NetworkManager::~NetworkManager(void)
{
	CleanUp();
}

NetworkManager* NetworkManager::GetThis()
{
	static NetworkManager instance;
	return &instance;
}

bool NetworkManager::Init()
{
	ZeroMemory(&m_cBuf, sizeof(m_cBuf));

	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
	{
		MessageBox(NULL,"WSAStartup Error !!",NULL,MB_OK);
		return false;
	}

	m_client_sock = socket(AF_INET, SOCK_STREAM, 0);
	if( m_client_sock == INVALID_SOCKET) 
	{
		MessageBox(NULL,"m_listen_sock Error !!",NULL,MB_OK);
		return false;
	}

	m_bOtherPlayer = false;

	return true;
}

void NetworkManager::CleanUp()
{
	closesocket(m_client_sock);

	WSACleanup();
}

bool NetworkManager::Connect( char* ip_addr /*= "127.0.0.1"*/, int port /*= 9000*/ )
{
	SOCKADDR_IN serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = inet_addr(ip_addr);

	if( SOCKET_ERROR == connect( m_client_sock,(SOCKADDR*)&serveraddr, sizeof(serveraddr) ) )
	{
		std::cout << "*** Connect Failed ***" << std::endl;
		return false ;
	}
	else
	{
		std::cout << "*** Connect Success ***" << std::endl;
	
		HANDLE  hThread;
		DWORD dwThreadID;

		hThread = (HANDLE)_beginthreadex (
			NULL, 0, 
			MsgReceiverThread, 
			NULL, 0, 
			(unsigned *)&dwThreadID
			);

		CloseHandle(hThread);

	}

	return true;
}

bool NetworkManager::MsgSender( packetHeader* packet )
{
	ZeroMemory(m_cBuf,sizeof(m_cBuf));

	switch ( packet->m_Type )
	{
	case PT_SETDOL:

		memcpy( m_cBuf, packet, sizeof(packetSetDol) );

		m_iError = send(m_client_sock, m_cBuf, sizeof(packetSetDol) , 0);

		break;
	}
	
	if( m_iError == SOCKET_ERROR)
	{
		MessageBox(NULL,"Send Error",NULL,MB_OK);

		return false;
	}

	return true;

}

int NetworkManager::MsgReceiver()
{
	int Bytes = 0;

	ZeroMemory(m_cBuf,sizeof(m_cBuf));

	Bytes = recvn(m_client_sock, m_cBuf, sizeof(packetHeader),  0);
	
	packetHeader packetheader;

	memcpy(&packetheader,m_cBuf,sizeof(packetHeader));
	//std::cout<<Bytes<<std::endl;
	if( Bytes == SOCKET_ERROR)
		return SOCKET_ERROR;
	else
	{
		switch ( packetheader.m_Type )
		{
		case PT_REGI:
			{
				std::cout<<"game_start"<<std::endl;
				m_bOtherPlayer = true;

				Bytes = recvn(m_client_sock, &m_cBuf[Bytes], packetheader.m_uiRemainsize,  0);

				packetStart Start;

				memcpy(&Start, m_cBuf, sizeof(packetStart));

				WINDOWS->GetGameManager()->SetMyDolColor(Start.m_iColor);

			}
			break;
		case PT_END:
			{
				Bytes = recvn(m_client_sock, &m_cBuf[Bytes], packetheader.m_uiRemainsize,  0);

				packetEnd End;

				memcpy(&End, m_cBuf, sizeof(packetEnd));

				WINDOWS->GetGameManager()->SetWinner(End.m_iWinner);

				return 0;
			}
			break;
		case PT_SETDOL:
			{
				Bytes = recvn(m_client_sock, &m_cBuf[Bytes], packetheader.m_uiRemainsize,  0);

				packetSetDol Dol;

				memcpy(&Dol, m_cBuf, sizeof(packetSetDol));

				WINDOWS->GetGameManager()->SetDol(Dol.m_iPosX,Dol.m_iPosY,Dol.m_iColor);
			}
			break;
		case PT_TURN:
			{
				Bytes = recvn(m_client_sock, &m_cBuf[Bytes], packetheader.m_uiRemainsize,  0);

				packetTurn Turn;

				memcpy(&Turn, m_cBuf, sizeof(packetTurn));

				WINDOWS->GetGameManager()->SetTurn(Turn.m_iTurn);

			}
			break;
		}
	}

	return Bytes;
}

int NetworkManager::recvn( SOCKET s, char*buf, int len, int flags )
{
	int received;
	char* ptr = buf;
	int left = len;

	while(left > 0)
	{
		received = recv(s,ptr,left,flags);
		if(received == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		else if ( received == 0 )
		{
			break;
		}

		left -= received;
		ptr += received;
	}

	return (len - left);
}

bool NetworkManager::WaitingPlayer()
{
	float fTime = 0.0f;
	int count = 0;
	
	std::cout << "플레이어 기다리는중...";

	while ( 1 )
	{
		if ( m_bOtherPlayer)
		{
			break;
		}

		if ( count > 50 )
		{
			std::cout << std::endl << "TimeOut" << std::endl;
			return false;
		}

		if ( fTime > 10 )
		{
			std::cout << ".";
			fTime = 0.0f;
			count++;
		}
		else
		{
			Sleep(100);
			fTime += 1.0f;
		}
		
	}

	return true;
}
