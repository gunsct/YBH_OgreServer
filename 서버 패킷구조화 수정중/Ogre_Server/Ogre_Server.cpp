#include "Ogre_Server.h"

// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

unsigned int WINAPI MsgReceiverThread(LPVOID param)
{
	Critical Cr;
	Socket_Cli SC;
	Parser Ps;


	SOCKET client_sock;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];



	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	while (1){
		// ������ �ޱ�
		retval = Ogre_Server::GetThis()->MsgReceiver(SC);
		
		// ���� ������ ���
		buf[retval] = '\0';
		printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port), buf);

		Cr.Enter();

		
		}
		// ������ ������
		//������ ���� Ŭ�� ���� ��ü���� �ٽ� �����Ѵ�, ���� Ŭ�� �����ص� ���������
		retval = strlen(buf);
		
		if (Ogre_Server::GetThis()->packetheader.m_Type == PT_CLIREGI){
			Ogre_Server::GetThis()->RegiCli.check_regi = true;
			Ogre_Server::GetThis()->RegiCli.cli_num = cli_num - 1;

			Ogre_Server::GetThis()->MsgSender(&Ogre_Server::GetThis()->RegiCli, client_sock);
		}
		else{
			Ogre_Server::GetThis()->MsgReceiver(client_sock);
		}
		Cr.Leave();
	}

	// closesocket()
	closesocket(client_sock);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));


	return 0;

}

Ogre_Server::Ogre_Server(){
	regi = false;
	cli_num = 0;
	WSAStartup(MAKEWORD(2, 2), &wsa) != 0;
}
Ogre_Server::~Ogre_Server(){
	// ���� ����
	WSACleanup();
}

Ogre_Server* Ogre_Server::GetThis()
{
	static Ogre_Server instance;
	return &instance;
}

void Ogre_Server::MsgSender(packetHeader* packet, SOCKET _client_sock)
{
	SOCKET cli_sock = _client_sock;
	ZeroMemory(buf, sizeof(buf));

	switch (packet->m_Type)
	{
	case PT_CLIREGI:
		memcpy(buf, packet, sizeof(packetClientRegi));
		retval = send(cli_sock, buf, sizeof(packetClientRegi), 0);
		break;

	case PT_SETPOS:
		memcpy(buf, packet, sizeof(packetSetPos));
		retval = send(cli_sock, buf, sizeof(packetSetPos), 0);
		break;

	case PT_END:
		memcpy(buf, packet, sizeof(packetEnd));
		retval = send(cli_sock, buf, sizeof(packetEnd), 0);
		break;
	}
}

int Ogre_Server::MsgReceiver(Socket _sc)
{
	int Bytes = 0;
	SOCKET SC = _sc;

	ZeroMemory(buf, sizeof(buf));

	Bytes = recv(client_sock, buf, sizeof(packetHeader), 0);

	memcpy(&packetheader, buf, sizeof(packetHeader));
	//std::cout<<Bytes<<std::endl;
	if (Bytes == SOCKET_ERROR)
		return SOCKET_ERROR;
	else
	{
		switch (packetheader.m_Type)
		{
		case PT_CLIREGI:
			Bytes = recv(client_sock, &buf[Bytes], packetheader.m_uiRemainsize, 0);
			memcpy(&RegiCli, buf, sizeof(packetStart));
			packetheader.m_Type = RegiCli.m_Type;

			regi = RegiCli.check_regi;

			if (regi == 0){
				SC.set_sock(client_sock);
				SC.set_num(cli_num);
				SC.set_sockaddr(clientaddr);
				cli_info.push_back(SC);
				cli_num++;
			}
			break;

		case PT_SETPOS:
			Bytes = recv(client_sock, &buf[Bytes], packetheader.m_uiRemainsize, 0);
			memcpy(&Pos, buf, sizeof(packetStart));

			packetheader.m_Type = Pos.m_Type;
			int max = cli_info.size();
			for (int i = 0; i < max; i++){

				MsgSender(&Pos, )
			}
			break;

		case PT_START:
			break;

		case PT_END://�׳� ���� �Լ��Ұž��� ���⼭ ������..
			Bytes = recv(client_sock, &buf[Bytes], packetheader.m_uiRemainsize, 0);
			memcpy(&End, buf, sizeof(packetEnd));
			packetheader.m_Type = End.m_Type;
			break;
		}
	}

	return Bytes;
}

void Ogre_Server::set_socket(){
	// socket()
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
}

void Ogre_Server::set_bind(){
	// bind()
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");
}

void Ogre_Server::set_listen(){
	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
}
void Ogre_Server::accept_transfer(){
	while (1){
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET){
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// ������ ����
		hThread = (HANDLE)_beginthreadex(
			NULL, 0,
			MsgReceiverThread,
			NULL, 0,
			NULL
			);

		CloseHandle(hThread);
	}

	// closesocket()
	closesocket(listen_sock);

}
int main(int argc, char *argv[])
{
	Ogre_Server OS;
	OS.set_socket();
	OS.set_bind();
	OS.set_listen();
	OS.accept_transfer();

	return 0;
}