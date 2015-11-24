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

		//받은 데이터에서 클라 넘버부분만 보내올때는 0일테니 벡터에서 검색해서 같은 아이피인거 찾아서 그 넘버로 대체해서 전체에 전송한다
		//buf[0~1]넘버가 00~99 이런식으로 지정될거란말이지 그럼 클라 넘버를 저기에 넣어서 값을 바꿔버리는거지뭐 클라 번호는 itoa로 이미 문자열로
		//바꿔진 상태일태고 그거 인덱스단위로 전해줘야할거같다


		// 데이터 보내기
		//받은걸 보낸 클라 빼고 전체에게 다시 전송한다, 보낸 클라 포함해도 상관없을듯
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

void Server::accept_set(){//accept에서 클라이언트 최초 정보 받는거 전부 벡터엔 넣자
	while (1){
		// accept()
		Socket_Cli SC;

		SC.addrlen = sizeof(SC.get_sockaddr());
		SC.set_sock(accept(server_sock, (SOCKADDR *)&(SC.get_sockaddr()), &SC.addrlen));

		if (server_sock == INVALID_SOCKET){
			err_display("accept()");
			break;
		}
		//accept 끝나고 이제 리스트에다 소캣 클래스를 넣어야함
		//파서에서 최초생성 조건 맞을때만 카운터 증가해서 넘버로 준다 그리고 벡터에 등록하는것도 마찬가지다.
		//if(파서 조건 맞으면) cli_num++
		//SC.set_num(cli_num);
		cli_info.push_back(SC);

		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(SC.get_sockaddr().sin_addr), ntohs(SC.get_sockaddr().sin_port));

		// 스레드 생성, 어짜피 모든 클라 정보다 써야하니 특정 소캣을 인자로 보낼 필요가없을거같다는 내판단은 잘못됬다.. 데이터는 받아야지
		hThread = CreateThread(NULL, 0, ProcessClient,
			(LPVOID)SC.get_sock(), 0, NULL);
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