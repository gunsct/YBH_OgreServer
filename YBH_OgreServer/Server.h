#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

#include "Error.h"
#include "Socket_client.h"

#define SERVERPORT 9000
#define BUFSIZE    512

vector <Socket_Cli> cli_info;//Ŭ�� ���� �����

//Ŭ���̾�Ʈ ����Ʈ�� �ʿ��� Ŭ���̾�Ʈ �������ִ� Ŭ������
class Server : public Error{
private:
	int retval;
	WSADATA wsa;
	SOCKET server_sock;
	SOCKADDR_IN serveraddr;

	int cli_num;

	/*SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;*/
	HANDLE hThread;//������� �޽��� ���޺κ��̶� �Ľ̺κ� ���� ���� ����غ��߰���
	//����Ʈ �߰��ϰ� ���⿡ Ŭ����� ����Ŭ���� �� ��������

public:
	Server();
	~Server();

	void socket_set();
	void bind_set();
	void listen_set();
	void accept_set();
};