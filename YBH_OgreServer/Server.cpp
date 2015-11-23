#include "Server.h"

// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
	Error ER;
	//여기부분을 이제.. 전역 리스트로 가져다가 해야겠지?
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	while (1){
		// 데이터 받기
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR){
			ER.err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port), buf);

		// 데이터 보내기
		retval = send(client_sock, buf, retval, 0);
		if (retval == SOCKET_ERROR){
			ER.err_display("send()");
			break;
		}
	}

	// closesocket()
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}


Server::Server(){
	WSAStartup(MAKEWORD(2, 2), &wsa);// 윈속 초기화
}

Server::~Server(){
	closesocket(server_sock);
	WSACleanup();// 윈속 종료
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
		//accept 끝나고 이제 리스트에다 소캣 클래스를 넣어야함

		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(SC.get_sockaddr().sin_addr), ntohs(SC.get_sockaddr().sin_port));

		// 스레드 생성
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