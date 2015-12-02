#include "Client.h"

// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
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
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

Client::Client(){
	WSAStartup(MAKEWORD(2, 2), &wsa);//���� �ʱ�ȭ

	retval = 0;
	player_num = 0;
	other_num = 0;
	regi = 0;
	x = 134.3235246f;
	y = 1124.2323433f;
	z = 42.42423423445f;
}

Client::~Client(){
	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
}

int Client::recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0){
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

void Client::set_socket(){
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
}

void Client::set_connet(){
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");
}

void Client::transfer(){
	// ������ ������ ���
	while (1){
		//���ſ����� ��ǥ ���������� �׽�Ʈ���� �ʿ����
		x += (float)(rand() % 10 - 5);
		y += (float)(rand() % 10 - 5);
		z += (float)(rand() % 10 - 5);

		// ������ �Է�
		printf("\n[���� ������] ");

		//��Ŷ ����
		strcpy(buf, Ps.packeting(player_num, other_num, regi, x, y, z));

		// ������ ������
		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR){
			err_display("send()");
			break;
		}
		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

		// ������ �ޱ�
		retval = recvn(sock, buf, retval, 0);
		if (retval == SOCKET_ERROR){
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// ���� ������ ���
		buf[retval] = '\0';
		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
		printf("[���� ������] %s\n", buf);

		//�Ľ� ����
		Ps.parsing_msg(buf);
		player_num = Ps.get_pn();
		other_num = Ps.get_on();
		regi = Ps.get_rg();
		x = Ps.get_xp();
		y = Ps.get_yp();
		z = Ps.get_zp();
		printf("%d %d %d %f %f %f", player_num, other_num, regi, x, y, z);

		regi = 1;
		Sleep(200);
	}
}

void Client::run_client(){
	set_socket();
	set_connet();
	transfer();
}

void Client::set_pos(float _x, float _y, float _z){
	x = _x;
	y = _y;
	z = _z;
}

float Client::get_xp(){
	return x;
}

float Client::get_yp(){
	return y;
}

float Client::get_zp(){
	return z;
}

int Client::get_pn(){
	return player_num;
}

int Client::get_on(){
	return other_num;
}

int main(int argc, char *argv[]){
	srand(time(NULL));//���ſ����� ��ǥ ���������� �׽�Ʈ���� �ʿ����
	Client ci;
	ci.run_client();
}