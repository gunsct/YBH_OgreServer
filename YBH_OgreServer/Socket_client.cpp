#include "Socket_client.h"

void Socket_Cli::set_num(int _num){
	num = _num;
}

int Socket_Cli::get_num(){
	return num;
}

void Socket_Cli::set_sock(SOCKET _CS){
	client_sock = _CS;
}

SOCKET Socket_Cli::get_sock(){
	return client_sock;
}


void Socket_Cli::set_sockaddr(SOCKADDR_IN _CSA){
	clientaddr = _CSA;
}

SOCKADDR_IN Socket_Cli::get_sockaddr(){
	return clientaddr;
}