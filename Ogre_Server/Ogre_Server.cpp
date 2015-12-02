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

// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI ProcessClient(LPVOID arg)
{
	Critical Cr;
	Socket_Cli SC;
	Parser Ps;
	

	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE+1];

	Cr.Enter();

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	while(1){
		Cr.Enter();
		// ������ �ޱ�
		buf[0] = '\0';
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if(retval == SOCKET_ERROR){
			err_display("recv()");
			break;
		}
		else if(retval == 0)
			break;

		if (Ps.parsing_msg(buf) == 0){
			SC.set_sock(client_sock);
			SC.set_num(cli_num);
			SC.set_sockaddr(clientaddr); 
			cli_info.push_back(SC);
			cli_num++;
		}
		// ���� ������ ���
		buf[retval] = '\0';
		printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port), buf);

		int max = cli_info.size();
		for (int i = 0; i < max; i++){
			if (cli_info[i].get_sockaddr().sin_port == SC.get_sockaddr().sin_port){//���Ϳ� �ִ� Ŭ���� ������ Ŭ��� ������ �����ѰŸ� �ѹ� ����Ŷ���� 
				strcpy_s(buf, Ps.re_packet_msg(buf, SC.get_num(), cli_info[i].get_num()));//��Ŷ�� ���� Ŭ���̾�Ʈ�� �ѹ��� ����Ŷ
			}
		}
		
		// ������ ������
		//������ ���� Ŭ�� ���� ��ü���� �ٽ� �����Ѵ�, ���� Ŭ�� �����ص� ���������
		retval = strlen(buf);
		for (int i = 0; i < max; i++){
			client_sock = cli_info[i].get_sock();//�������� ���Ϳ��� ���� ������ �޾ƿ�

			//�̰� ������� ���� Ŭ�� ������� �� �� �ִ�
			strcpy_s(buf, Ps.re_packet_msg(buf, cli_info[i].get_num(), -1));//��Ŷ�� �޴� Ŭ���̾�Ʈ�� �ѹ��� ����Ŷ, ���� ������ �ѹ� -1�� �������ָ� ��

			retval = send(client_sock, buf, retval, 0);
			if (retval == SOCKET_ERROR){
				err_display("send()");
				break;
			}
		}
	}

	// closesocket()
	closesocket(client_sock);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	Cr.Leave();
	return 0;
}

Ogre_Server::Ogre_Server(){
	WSAStartup(MAKEWORD(2, 2), &wsa) != 0;
}
Ogre_Server::~Ogre_Server(){
	// ���� ����
	WSACleanup();
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
		hThread = CreateThread(NULL, 0, ProcessClient,
			(LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
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