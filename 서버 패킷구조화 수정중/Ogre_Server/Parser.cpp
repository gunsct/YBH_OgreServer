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

int Parser::parsing_msg(char* buf){//���⵵ Ŭ��ó�� ��ǥ �ڸ��� ��������ߵ� �켱 6�ڸ� �����غ���
	int point_num = 0;//�Ź� �� �Լ� �θ������� �翬�� �� �ʱ�ȭ �Ǿߵ�
	int buf_pos = 0;

	init();//Ȥ�ø𸣴� �ٽ� �� �ʱ�ȭ

	//��Ŷ����
	// renum senum regi x y z
	// 0	 0	   0/1	0 0 0
	// 0	12	  34   5678910
	for (int i = 0; i < sizeof(buf)*9; i++){//�� ��Ҹ� �����ؼ� ���ڹ迭�� �־��� �̰� �ٸ� �ڷ������� �ٵ� ��ȯ�Ұ���
		if (buf[i] == ' '){ //���� �������� ��Ŷ�� ��ҷ� ������
			point_num++;//��� ���� �÷���, ����Ǻ������� ����...
			if (point_num == 3)
				restart = i;
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
			}
		}
	}

	//�� ���� �迭�� �� ��ҵ� �ڷ�����ȯ�ؾ���
	//cli_num = atoi(pnum);
	regist = atoi(regi);
	//xpos = atof(pxp);
	//ypos = atof(pyp);
	//zpos = atof(pzp);

	return regist;
}

const char* Parser::re_packet_msg(char* buf, int re_cli_num, int se_cli_num){//����Ŷ
	char temp[512] = "";
	int size = sizeof(buf);
	if (se_cli_num == -1){//��Ŷ�� �޴� Ŭ���̾�Ʈ�� �ѹ��� ����Ŷ
		for (int i = 0; i < ((sizeof(buf) + 2) * 8 - restart + 2); i++){
			temp[i] = buf[restart - 3 + i];
		}

		sprintf_s(rebuf, "%d %s", re_cli_num, temp);
	}

	else{//ó�� ����ϰ� �����ַ��Ҷ� ��ǥ ������ �ѹ� ����Ŷ
		for (int i = 0; i < ((sizeof(buf) + 2) * 8 - restart); i++){
			temp[i] = buf[restart - 1 + i];
		}

		sprintf_s(rebuf, "%d %d %s", re_cli_num, se_cli_num, temp);
	}
	return rebuf;
}