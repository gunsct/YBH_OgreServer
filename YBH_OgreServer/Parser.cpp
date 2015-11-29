#include "Parser.h"

Parser::Parser(){}

Parser::~Parser(){}

int Parser::parsing_msg(char* buf){
	int point_num = 0;//매번 이 함수 부를때마다 당연히 다 초기화 되야됨
	int buf_pos = 0;
	char pnum[2] = "";
	char pregi[1] = "";

	if (point_num < 3){
		for (int i = 0; i < sizeof(buf); i++){//각 요소를 분할해서 문자배열에 넣어줌 이걸 다른 자료형으로 다들 변환할거임
			if (buf[i] == ' '){ //띄어쓰기 구분으로 패킷을 요소로 나누고
				if (point_num == 2)
					restart = i;
				point_num++;//요소 단위 올려줌, 등록판별까지만 보자...
				buf_pos = 0;//요소 버퍼위치를 다시 0으로
			}

			else{//띄어쓰기 아닐때만 요소 분할된거 값을 넣어줌
				switch (point_num){
				case 0:
					pnum[buf_pos] = buf[i];
					buf_pos++;
					break;

				case 1:
					pregi[buf_pos] = buf[i];
					buf_pos++;
					break;
				}
			}
		}
	}

	//각 문자 배열로 들어간 요소들 자료형변환해야함
	//cli_num = atoi(pnum);
	regist = atoi(pregi);
	//xpos = atof(pxp);
	//ypos = atof(pyp);
	//zpos = atof(pzp);

	return regist;
}

const char* Parser::re_packet_msg(char* buf, int cli_num){//리패킷
	char temp[512] = "";
	for (int i = 0; i < ((char)sizeof(buf) - restart); i++){
		temp[i] = buf[restart + i];
	}

	sprintf_s(rebuf, "%d %s", cli_num, temp);

	return rebuf;
}