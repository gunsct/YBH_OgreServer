#include "Parser.h"

Parser::Parser(){
	for (int i = 0; i < 4;i++)
		buf[i] = ' ';
}

Parser::~Parser(){}

void Parser::num_regist(Socket_Cli SC){
	//소켓 클래스 받아서 실제로 파싱해서 필요한 변환 구현부분
}