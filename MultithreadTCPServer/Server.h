#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

#include "Error.h"
#include "Socket_client.h"
#include "Parser.h"

#define SERVERPORT 9000
#define BUFSIZE    512

vector <Socket_Cli> cli_info;//Ŭ�� ���� �����
int cli_num = 0;
//Ŭ���̾�Ʈ ����Ʈ�� �ʿ��� Ŭ���̾�Ʈ �������ִ� Ŭ������
class Server : public Error{
private:
	int retval;
	WSADATA wsa;
	SOCKET server_sock;
	SOCKADDR_IN serveraddr;

	/*SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;*/
	HANDLE hThread;//������� �޽��� ���޺κ��̶� �Ľ̺κ� ���� ���� �����غ��߰���
	//����Ʈ �߰��ϰ� ���⿡ Ŭ����� ����Ŭ���� �� ��������

public:
	Server();
	~Server();

	void socket_set();
	void bind_set();
	void listen_set();
	void accept_set();

	//������ �׳� �Ľ��̶� ��Ŷ���� ���⿡ �ִ°� �����Ű����� �ƴѰ�? ���� �Լ������� ���� �������ϴ°�..��?
	void set_packet();
	void parsing_msg(char packet);//��Ŷ �����ؼ� ���� �迭 ��� �ڷ�����ȯ�ؼ� ���� ����� �ٲ���
};