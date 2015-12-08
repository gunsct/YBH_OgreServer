#include "Parser.h"

Parser::Parser(){
}

Parser::~Parser(){

}

const char* Parser::packeting(int pn, int on, int regi, float x, float y, float z){
	char buf[100];
	buf[0] = '\0';
	//float �ڷ����� ������ �ڸ����������ִ°� �߿��ߴ�
	sprintf(buf, "%d %d %d %010.3f %010.3f %010.3f", pn, on, regi, x, y, z);
	return buf;
}

void Parser::parsing_msg(char* buf){//���⵵ Ŭ��ó�� ��ǥ �ڸ��� ��������ߵ� �켱 6�ڸ� �����غ���
	char player_num[1] = "";
	char other_num[1] = "";
	char regi[1] = "";
	char pxp[20] = "";
	char pyp[20] = "";
	char pzp[20] = "";

	int point_num = 0;//�Ź� �� �Լ� �θ������� �翬�� �� �ʱ�ȭ �Ǿߵ�
	int buf_pos = 0;


	//��Ŷ����
	// renum senum regi x y z
	// 0	 0	   0/1	0 0 0
	// 0	12	  34   5678910
	for (int i = 0; i < sizeof(buf) * 9; i++){//�� ��Ҹ� �����ؼ� ���ڹ迭�� �־��� �̰� �ٸ� �ڷ������� �ٵ� ��ȯ�Ұ���
		if (buf[i] == ' '){ //���� �������� ��Ŷ�� ��ҷ� ������
			point_num++;//��� ���� �÷���, ����Ǻ������� ����...
			buf_pos = 0;//��� ������ġ�� �ٽ� 0����
		}

		else{//���� �ƴҶ��� ��� ���ҵȰ� ���� �־���
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

	//�� ���� �迭�� �� ��ҵ� �ڷ�����ȯ�ؾ���
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