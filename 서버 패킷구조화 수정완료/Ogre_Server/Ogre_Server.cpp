#include "Ogre_Server.h"

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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
	SOCKET cs = (SOCKET)param;
	Critical Cr;
	Socket_Cli SC;
	Parser Ps;

	int retval;

	while (1){
		// 데이터 받기
		retval = Ogre_Server::GetThis()->MsgReceiver(cs);
		
		// 데이터 보내기
		//받은걸 보낸 클라 빼고 전체에게 다시 전송한다, 보낸 클라 포함해도 상관없을듯
		

		int max = cli_info.size();
		for (int i = 0; i < max; i++){
			if (Ogre_Server::GetThis()->packetheader.m_Type == PT_CLIREGI){
				if (cli_info[i].get_sock() == cs){
					//packetClientRegi RegiCli(cli_info[i].get_num(), false);
					Ogre_Server::GetThis()->RegiCli.check_regi = false;
					Ogre_Server::GetThis()->RegiCli.cli_num = cli_info[i].get_num();
					Ogre_Server::GetThis()->MsgSender(&(Ogre_Server::GetThis()->RegiCli), cli_info[i].get_sock());
				}
			}

			else if (Ogre_Server::GetThis()->start_on == true){
				packetStart Start(2);
				/*Ogre_Server::GetThis()->Start.fugitive_num = 0;*/

				Ogre_Server::GetThis()->MsgSender(&Start, cli_info[i].get_sock());

				if(i>1)Ogre_Server::GetThis()->start_on = false;
			}
			
			else{
				//packetSetPos Pos;
				Ogre_Server::GetThis()->MsgSender(&Ogre_Server::GetThis()->Pos, cli_info[i].get_sock());
			}
		}
	}

	// closesocket()
	closesocket(Ogre_Server::GetThis()->client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(Ogre_Server::GetThis()->clientaddr.sin_addr), ntohs(Ogre_Server::GetThis()->clientaddr.sin_port));


	return 0;

}

Ogre_Server::Ogre_Server(){
	regi = false;
	cli_num = 0;
	start_on = false;
	RegiCli.m_Type = PT_CLIREGI;
	End.m_Type = PT_END;
	Start.m_Type = PT_START;
	Pos.m_Type = PT_SETPOS;
	WSAStartup(MAKEWORD(2, 2), &wsa) != 0;

	/*packetHeader packetheader(PT_NONE, 0);
	packetClientRegi RegiCli(0, false);
	packetStart Start(0);
	packetEnd End(0, 0);
	packetSetPos Pos(0, 0, 0, 0);*/
}
Ogre_Server::~Ogre_Server(){
	// 윈속 종료
	WSACleanup();
}

Ogre_Server* Ogre_Server::GetThis()
{
	static Ogre_Server instance;
	return &instance;
}

void Ogre_Server::MsgSender(packetHeader* packet, SOCKET _client_sock)
{
	Cr.Enter();
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

	case PT_START:
		memcpy(buf, packet, sizeof(packetStart));
		retval = send(cli_sock, buf, sizeof(packetStart), 0);
		break;

	case PT_END:
		memcpy(buf, packet, sizeof(packetEnd));
		retval = send(cli_sock, buf, sizeof(packetEnd), 0);
		break;
	}
	Cr.Leave();
}

int Ogre_Server::MsgReceiver(SOCKET _cs)
{
	SOCKET cs = _cs;
	Socket_Cli SC;
	ZeroMemory(buf, sizeof(buf));

	retval = recv(cs, buf, sizeof(packetHeader), 0);

	

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(cs, (SOCKADDR *)&clientaddr, &addrlen);
	memcpy(&packetheader, buf, sizeof(packetHeader));

	//std::cout<<Bytes<<std::endl;
	if (retval == SOCKET_ERROR)
		return SOCKET_ERROR;
	else
	{
		switch (packetheader.m_Type)
		{
		case PT_CLIREGI:
			retval = recv(cs, &buf[retval], packetheader.m_uiRemainsize, 0);
			memcpy(&RegiCli, buf, sizeof(packetClientRegi));

			regi = RegiCli.check_regi;

			if (regi == true){
				SC.set_sock(cs);
				SC.set_num(cli_num);
				SC.set_sockaddr(clientaddr);
				cli_info.push_back(SC);
				cli_num++;
				if (cli_num == 3) start_on = true;
			}
			printf("%d %d\n", RegiCli.check_regi, RegiCli.cli_num);
			break;

		case PT_SETPOS:
			retval = recv(cs, &buf[retval], packetheader.m_uiRemainsize, 0);
			memcpy(&Pos, buf, sizeof(packetSetPos));
			printf("%d %f %f %f\n", Pos.cli_num, Pos.xPos, Pos.yPos, Pos.zPos);
			break;

		case PT_START:
			retval = recv(cs, &buf[retval], packetheader.m_uiRemainsize, 0);
			memcpy(&Pos, buf, sizeof(packetStart));

			break;

		case PT_END://그냥 따로 함수할거없이 여기서 끝내자..
			retval = recv(cs, &buf[retval], packetheader.m_uiRemainsize, 0);
			memcpy(&End, buf, sizeof(packetEnd));

			break;
		}
	}
	// 받은 데이터 출력
	buf[retval] = '\0';
	printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
		ntohs(clientaddr.sin_port), buf);


	return retval;
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

		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 스레드 생성
		hThread = (HANDLE)_beginthreadex(
			NULL, 0,
			MsgReceiverThread,
			(LPVOID)client_sock, 0,
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