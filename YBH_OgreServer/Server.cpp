#include "Server.h"

// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI ProcessClient(LPVOID arg)
{
	Error ER;

	Socket_Cli SC;//����κ��� ����.. ���� ����Ʈ�� �����ٰ� �ؾ߰���?
	Parser Ps;

	SC.set_sock((SOCKET)arg);
	int retval;
	char buf[BUFSIZE + 1];

	// Ŭ���̾�Ʈ ���� ���
	SC.addrlen = sizeof(SC.get_sockaddr());
	getpeername(SC.get_sock(), (SOCKADDR *)&SC.get_sockaddr(), &SC.addrlen);

	while (1){
		// ������ �ޱ�
		retval = recv(SC.get_sock(), buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR){
			ER.err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		//accept�ϰ� �������ߴ��� ��Ŷ�� �޾Ƽ� �ؼ��Ѵ����� �ؾ��ϴ°Ŷ� ���⼭ ��
		//�ļ����� ���ʻ��� ���� �������� ī���� �����ؼ� �ѹ��� �ش� �׸��� ���Ϳ� ����ϴ°͵� ����������.
		//if(�ļ� ���� ������) 
		if (Ps.parsing_msg(buf) == 0){
			SC.set_num(cli_num);
			cli_info.push_back(SC);
			cli_num++;
		}
		

		// ���� ������ ���
		buf[retval] = '\0';
		/*printf("[TCP/%s:%d] %s\n", inet_ntoa(SC.get_sockaddr().sin_addr),
			ntohs(SC.get_sockaddr().sin_port), buf);*/

		//���� �����Ϳ��� Ŭ�� �ѹ��κи� �����ö��� 0���״� ���Ϳ��� �˻��ؼ� ���� �������ΰ� ã�Ƽ� �� �ѹ��� ��ü�ؼ� ��ü�� �����Ѵ�
		//buf[0~1]�ѹ��� 00~99 �̷������� �����ɰŶ������� �׷� Ŭ�� �ѹ��� ���⿡ �־ ���� �ٲ�����°����� Ŭ�� ��ȣ�� itoa�� �̹� ���ڿ���
		//�ٲ��� �������°� �װ� �ε��������� ��������ҰŰ���
		int max = cli_info.size();
		for (int i = 0; i < max; i++){
			//getpeername(cli_info[i].get_sock(), (SOCKADDR *)&cli_info[i].get_sockaddr(), &cli_info[i].addrlen);
			if (cli_info[i].get_sockaddr().sin_port == SC.get_sockaddr().sin_port){//���Ϳ� �ִ� Ŭ���� ������ Ŭ��� ������ �����ѰŸ� �ѹ� ����Ŷ���� 
				strcpy_s(buf, Ps.re_packet_msg(buf, cli_info[i].get_num()));
			}
		}
		// ������ ������
		//������ ���� Ŭ�� ���� ��ü���� �ٽ� �����Ѵ�, ���� Ŭ�� �����ص� ���������
		for (int i = 0; i < max; i++){
			retval = send(cli_info[i].get_sock(), buf, retval, 0);
			if (retval == SOCKET_ERROR){
				ER.err_display("send()");
				break;
			}
		}
	}

	// closesocket()
	closesocket(SC.get_sock());
	/*printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(SC.get_sockaddr().sin_addr), ntohs(SC.get_sockaddr().sin_port));*/

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

void Server::accept_set(){//accept���� Ŭ���̾�Ʈ ���� ���� �޴°� ���� ���Ϳ� ����
	while (1){
		// accept()
		Socket_Cli SC;

		SC.addrlen = sizeof(SC.get_sockaddr());
		SC.set_sock(accept(server_sock, (SOCKADDR *)&(SC.get_sockaddr()), &SC.addrlen));

		if (server_sock == INVALID_SOCKET){
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
	/*	printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(SC.get_sockaddr().sin_addr), ntohs(SC.get_sockaddr().sin_port));*/

		// ������ ����, ��¥�� ��� Ŭ�� ������ ����ϴ� Ư�� ��Ĺ�� ���ڷ� ���� �ʿ䰡�����Ű��ٴ� ���Ǵ��� �߸����.. �����ʹ� �޾ƾ���
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)SC.get_sock(), 0, NULL);
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