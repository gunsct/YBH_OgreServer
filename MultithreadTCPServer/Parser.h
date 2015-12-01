//패킷 분석하는부분 클라이언트쪽에서 짜야할거같다
//아마 나는 패킷을 클라넘버,최초 생성여부,좌표 로 구성하고 이걸 특수문자나 띄어쓰기 단위로 따로 따로 분석해서 자료형에 맞춰 하지않을까 한다
//패킷 맨처음에 넘버랑 생성부분 세자리만 체크하면 됨 서버는 이거 3개만 필요 클라나 복잡하게 짜야지..
#include"Socket_client.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>

class Parser{
private:
	char rebuf[513];//리패킷할때 쓰일 버퍼
	int restart;//패킷 재설계할때 이어붙이기 시작할곳
	//int cli_num; 전역변수 쓸거임
	int regist;//벡터에 최초등록할지 판별할때 패킷에서 값 받는것

	//패킷용 변수들
	char player_num[1];//받는쪽
	char other_num[1];//보내는쪽
	char regi[1];
	char xp[20];
	char yp[20];
	char zp[20];

public:
	Parser();
	~Parser();
	void init();
	int parsing_msg(char* buf);//등록여부만 리턴함
	const char* re_packet_msg(char* buf,int re_cli_num, int se_cli_num);//받는사람 보내는사람 넘버 리패킷해줌
};

