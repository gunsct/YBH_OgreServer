#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512

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

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while(left > 0){
		received = recv(s, ptr, left, flags);
		if(received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if(received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

int main(int argc, char *argv[])
{
	int retval;
	int player_num = 0;
	int other_num = 0;
	int regi = 0;
	float x = 134.3235246f;
	float y = 1124.2323433f;
	float z = 42.42423423445f;
	srand(time(NULL));
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("connect()");

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE+1];
	int len;

	// ������ ������ ���
	while(1){
		x += (float)(rand() % 10 -5);
		y += (float)(rand() % 10 - 5);
		z += (float)(rand() % 10 - 5);

		char cx[20];
		cx[0] = '\0';
		char cy[20];
		cy[0] = '\0';
		char cz[20];
		cz[0] = '\0';
		sprintf(cx, "%f", x);
		sprintf(cy, "%f", y);
		sprintf(cz, "%f", z);
		/*for (int i = 0; i < 16; i++){
			ccx[i] = cx[i];
			ccy[i] = cy[i];
			ccz[i] = cz[i];
		}*/
		// ������ �Է�
		printf("\n[���� ������] ");
		/*if(fgets(buf, BUFSIZE+1, stdin) == NULL)
			break;*/
		sprintf(buf, "%d %d %d %010.3f %010.3f %010.3f", player_num, other_num, regi, x, y, z);

		//// '\n' ���� ����
		//len = strlen(buf);
		//if(buf[len-1] == '\n')
		//	buf[len-1] = '\0';
		//if(strlen(buf) == 0)
		//	break;
		// ������ ������
		retval = send(sock, buf, strlen(buf), 0);
		if(retval == SOCKET_ERROR){
			err_display("send()");
			break;
		}
		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

		// ������ �ޱ�
		retval = recvn(sock, buf, retval, 0);
		if(retval == SOCKET_ERROR){
			err_display("recv()");
			break;
		}
		else if(retval == 0)
			break;

		// ���� ������ ���
		buf[retval] = '\0';
		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
		printf("[���� ������] %s\n", buf);

		regi = 1;
		Sleep(200);
	}

	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}