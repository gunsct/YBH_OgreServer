#include "Parser.h"

Parser::Parser(){
}

Parser::~Parser(){

}

const char* Parser::packeting(int pn, int on, int regi, float x, float y, float z){
	char buf[100];
	buf[0] = '\0';
	//float 자료형의 값들을 자리수고정해주는게 중요했다
	sprintf(buf, "%d %d %d %010.3f %010.3f %010.3f", pn, on, regi, x, y, z);
	return buf;
}

void Parser::parsing_msg(char* buf){//여기도 클라처럼 좌표 자리수 고정해줘야되 우선 6자리 고정해보자
	char player_num[1] = "";
	char other_num[1] = "";
	char regi[1] = "";
	char pxp[20] = "";
	char pyp[20] = "";
	char pzp[20] = "";

	int point_num = 0;//매번 이 함수 부를때마다 당연히 다 초기화 되야됨
	int buf_pos = 0;


	//패킷구조
	// renum senum regi x y z
	// 0	 0	   0/1	0 0 0
	// 0	12	  34   5678910
	for (int i = 0; i < sizeof(buf) * 9; i++){//각 요소를 분할해서 문자배열에 넣어줌 이걸 다른 자료형으로 다들 변환할거임
		if (buf[i] == ' '){ //띄어쓰기 구분으로 패킷을 요소로 나누고
			point_num++;//요소 단위 올려줌, 등록판별까지만 보자...
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

			case 3:
				pxp[buf_pos] = buf[i];
				buf_pos++;
				break;

			case 4:
				pyp[buf_pos] = buf[i];
				buf_pos++;
				break;

			case 5:
				pzp[buf_pos] = buf[i];
				buf_pos++;
				break;
			}
		}
	}

	//각 문자 배열로 들어간 요소들 자료형변환해야함
	pn = atoi(player_num);
	on = atoi(other_num);
	rg = atoi(regi);
	xp = atof(pxp);
	yp = atof(pyp);
	zp = atof(pzp);
}

int Parser::get_pn(){
	return pn;
}

int Parser::get_on(){
	return on;
}

int Parser::get_rg(){
	return rg;
}

float Parser::get_xp(){
	return xp;
}

float Parser::get_yp(){
	return yp;
}

float Parser::get_zp(){
	return zp;
}