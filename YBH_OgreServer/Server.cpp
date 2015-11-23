#include "Server.h"

// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI ProcessClient(LPVOID arg)
{
	Error ER;
	//����κ��� ����.. ���� ����Ʈ�� �����ٰ� �ؾ߰���?
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	while (1){
		// ������ �ޱ�
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR){
			ER.err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// ���� ������ ���
		buf[retval] = '\0';
		printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port), buf);

		// ������ ������
		retval = send(client_sock, buf, retval, 0);
		if (retval == SOCKET_ERROR){
			ER.err_display("send()");
			break;
		}
	}

	// closesocket()
	closesocket(client_sock);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}


Server::Server(){
	WSAStartup(MAKEWORD(2, 2), &wsa);// ���� �ʱ�ȭ
}

Server::~Server(){
	closesocket(server_sock);
	WSACleanup();// ���� ����
}

void Server::socket_set(){
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock == INVALID_SOCKET) err_quit("socket()");
}

void Server::bind_set(){
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(server_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");
}

void Server::listen_set(){
	retval = listen(server_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
}

void Server::accept_set(){
	while (1){
		// accept()
		SC.addrlen = sizeof(SC.get_sockaddr());
		SC.set_sock(accept(server_sock, (SOCKADDR *)&(SC.get_sockaddr()), &SC.addrlen));

		if (server_sock == INVALID_SOCKET){
			err_display("accept()");
			break;
		}
		//accept ������ ���� ����Ʈ���� ��Ĺ Ŭ������ �־����

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(SC.get_sockaddr().sin_addr), ntohs(SC.get_sockaddr().sin_port));

		// ������ ����
		hThread = CreateThread(NULL, 0, ProcessClient,
			0, 0, NULL);
		if (hThread == NULL) { closesocket(server_sock); }
		else { CloseHandle(hThread); }
	}
}



int main(int argc, char *argv[]){
	Server sv;

	sv.socket_set();
	sv.listen_set();
	sv.bind_set();

	return 0;
}