#include "Client.h"

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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
	WSAStartup(MAKEWORD(2, 2), &wsa);//윈속 초기화

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

	// 윈속 종료
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
	// 서버와 데이터 통신
	while (1){
		//오거에서는 좌표 직접받으니 테스트용은 필요없어
		x += (float)(rand() % 10 - 5);
		y += (float)(rand() % 10 - 5);
		z += (float)(rand() % 10 - 5);

		// 데이터 입력
		printf("\n[보낼 데이터] ");

		//패킷 제작
		strcpy(buf, Ps.packeting(player_num, other_num, regi, x, y, z));

		// 데이터 보내기
		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR){
			err_display("send()");
			break;
		}
		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		// 데이터 받기
		retval = recvn(sock, buf, retval, 0);
		if (retval == SOCKET_ERROR){
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
		printf("[받은 데이터] %s\n", buf);

		//파싱 구현
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
	srand(time(NULL));//오거에서는 좌표 직접받으니 테스트용은 필요없어
	Client ci;
	ci.run_client();
}