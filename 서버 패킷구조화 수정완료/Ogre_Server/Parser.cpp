#include "Parser.h"

Parser::Parser(){
	rebuf[0] = '\0';
	restart = 0;
	regist = 0;
	player_num[0] = '\0';
	other_num[0] = '\0';
	regi[0] = '\0';
	xp[0] = '\0';
	yp[0] = '\0';
	zp[0] = '\0';
}

Parser::~Parser(){}
void Parser::init(){
	rebuf[0] = '\0';
	restart = 0;
	regist = 0;
	player_num[0] = '\0';
	other_num[0] = '\0';
	regi[0] = '\0';
	xp[0] = '\0';
	yp[0] = '\0';
	zp[0] = '\0';
}

int Parser::parsing_msg(char* buf){//여기도 클라처럼 좌표 자리수 고정해줘야되 우선 6자리 고정해보자
	int point_num = 0;//매번 이 함수 부를때마다 당연히 다 초기화 되야됨
	int buf_pos = 0;

	init();//혹시모르니 다시 다 초기화

	//패킷구조
	// renum senum regi x y z
	// 0	 0	   0/1	0 0 0
	// 0	12	  34   5678910
	for (int i = 0; i < sizeof(buf)*9; i++){//각 요소를 분할해서 문자배열에 넣어줌 이걸 다른 자료형으로 다들 변환할거임
		if (buf[i] == ' '){ //띄어쓰기 구분으로 패킷을 요소로 나누고
			point_num++;//요소 단위 올려줌, 등록판별까지만 보자...
			if (point_num == 3)
				restart = i;
			buf_pos = 0;//요소 버퍼위치를 다시 0으로
		}

		else{//띄어쓰기 아닐때만 요소 분할된거 값을 넣어줌
			switch (point_num){
			case 0:
				player_num[buf_pos] = buf[i];
				buf_pos++;
				break;

			case 1:
				other_num[buf_pos] = buf[i];
				buf_pos++;
				break;

			case 2:
				regi[buf_pos] = buf[i];
				buf_pos++;
				break;
			}
		}
	}

	//각 문자 배열로 들어간 요소들 자료형변환해야함
	//cli_num = atoi(pnum);
	regist = atoi(regi);
	//xpos = atof(pxp);
	//ypos = atof(pyp);
	//zpos = atof(pzp);

	return regist;
}

const char* Parser::re_packet_msg(char* buf, int re_cli_num, int se_cli_num){//리패킷
	char temp[512] = "";
	int size = sizeof(buf);
	if (se_cli_num == -1){//패킷을 받는 클라이언트의 넘버를 리패킷
		for (int i = 0; i < ((sizeof(buf) + 2) * 8 - restart + 2); i++){
			temp[i] = buf[restart - 3 + i];
		}

		sprintf_s(rebuf, "%d %s", re_cli_num, temp);
	}

	else{//처음 등록하고 보내주려할때 좌표 정보자 넘버 리패킷
		for (int i = 0; i < ((sizeof(buf) + 2) * 8 - restart); i++){
			temp[i] = buf[restart - 1 + i];
		}

		sprintf_s(rebuf, "%d %d %s", re_cli_num, se_cli_num, temp);
	}
	return rebuf;
}