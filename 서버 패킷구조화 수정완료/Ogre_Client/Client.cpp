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
	player_num = other_num = 0;
	fugitive_num = catcher_num = 0;
	regi = true;
	endflag = false;
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

void Client::MsgSender(packetHeader* packet)
{
	Cr.Enter();

	ZeroMemory(buf, sizeof(buf));

	switch (packet->m_Type)
	{
	case PT_CLIREGI:
		memcpy(buf, packet, sizeof(packetClientRegi));
		retval = send(sock, buf, sizeof(packetClientRegi), 0);
		break;

	case PT_SETPOS:
		memcpy(buf, packet, sizeof(packetSetPos));
		retval = send(sock, buf, sizeof(packetSetPos), 0);
		break;

	case PT_END:
		memcpy(buf, packet, sizeof(packetEnd));
		retval = send(sock, buf, sizeof(packetEnd), 0);
		break;
	}
	// ������ ������
	printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);
	printf("[���� ������] %s\n", buf);

	Cr.Leave();
}

int Client::MsgReceiver()
{
	int Bytes = 0;

	ZeroMemory(buf, sizeof(buf));

	Bytes = recvn(sock, buf, sizeof(packetHeader), 0);

	packetHeader packetheader;

	memcpy(&packetheader, buf, sizeof(packetHeader));
	//std::cout<<Bytes<<std::endl;
	if (Bytes == SOCKET_ERROR)
		return SOCKET_ERROR;
	else
	{
		switch (packetheader.m_Type)
		{
		case PT_CLIREGI:
			
			Bytes = recvn(sock, &buf[Bytes], packetheader.m_uiRemainsize, 0);

			memcpy(&RegiCli, buf, sizeof(packetClientRegi));

			player_num = RegiCli.cli_num;
			regi = RegiCli.check_regi;

			// ���� ������ ���
			printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", Bytes);
			printf("[���� ������] %d %d\n", player_num, regi);
			break;

		case PT_SETPOS:
			Bytes = recvn(sock, &buf[Bytes], packetheader.m_uiRemainsize, 0);

			
			memcpy(&Pos, buf, sizeof(packetSetPos));

			other_num = Pos.cli_num;
			x = Pos.xPos;
			y = Pos.yPos;
			z = Pos.zPos;
			// ���� ������ ���
			printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", Bytes);
			printf("[���� ������] %d %f %f %f\n", other_num, x, y, z);
			break;

		case PT_START:
			Bytes = recvn(sock, &buf[Bytes], packetheader.m_uiRemainsize, 0);

			

			memcpy(&Start, buf, sizeof(packetStart));

			start = clock();
			fugitive_num =  Start.fugitive_num;
			// ���� ������ ���
			printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", Bytes);
			printf("[���� ������] %d\n", fugitive_num);
			break;

		case PT_END://�׳� ���� �Լ��Ұž��� ���⼭ ������..
			Bytes = recvn(sock, &buf[Bytes], packetheader.m_uiRemainsize, 0);
			
		

			memcpy(&End, buf, sizeof(packetEnd));

			catcher_num = End.catcher_num;//������� ��ȣ
			duration = End.cTime;

			// ���� ������ ���
			printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", Bytes);
			printf("[���� ������] %d %f\n", catcher_num, duration);
			break;
		}
	}

	return Bytes;
}
void Client::transfer(){
	// ������ ������ ���
	while (1){

		//���ſ����� ��ǥ ���������� �׽�Ʈ���� �ʿ����
		x = 134.3235246f;
		y = 1124.2323433f;
		z = 42.42423423445f;
		x += (float)(rand() % 10 - 5);
		y += (float)(rand() % 10 - 5);
		z += (float)(rand() % 10 - 5);

		// ������ �Է�
		//printf("\n[���� ������] ");

		
		if (regi == true){//������Ʈ ���� ���
			packetClientRegi RegiCli(0, regi);
			MsgSender(&RegiCli);
		}

		else{
			if (endflag == false){
				packetSetPos Pos(player_num, x, y, z);
				MsgSender(&Pos);
			}

			else{
				finish = clock();
				duration = (double)(finish - start) / CLOCKS_PER_SEC;

				packetEnd End(catcher_num, duration);
				MsgSender(&End);
			}
		}

		
		// ������ �ޱ�
		MsgReceiver();

		
		//Sleep(500);
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
void Client::set_flag(bool _flag){
	endflag = _flag;
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