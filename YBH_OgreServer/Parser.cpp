#include "Parser.h"

Parser::Parser(){}

Parser::~Parser(){}

int Parser::parsing_msg(char* buf){
	int point_num = 0;//�Ź� �� �Լ� �θ������� �翬�� �� �ʱ�ȭ �Ǿߵ�
	int buf_pos = 0;
	char pnum[2] = "";
	char pregi[1] = "";

	if (point_num < 3){
		for (int i = 0; i < sizeof(buf); i++){//�� ��Ҹ� �����ؼ� ���ڹ迭�� �־��� �̰� �ٸ� �ڷ������� �ٵ� ��ȯ�Ұ���
			if (buf[i] == ' '){ //���� �������� ��Ŷ�� ��ҷ� ������
				if (point_num == 2)
					restart = i;
				point_num++;//��� ���� �÷���, ����Ǻ������� ����...
				buf_pos = 0;//��� ������ġ�� �ٽ� 0����
			}

			else{//���� �ƴҶ��� ��� ���ҵȰ� ���� �־���
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

	//�� ���� �迭�� �� ��ҵ� �ڷ�����ȯ�ؾ���
	//cli_num = atoi(pnum);
	regist = atoi(pregi);
	//xpos = atof(pxp);
	//ypos = atof(pyp);
	//zpos = atof(pzp);

	return regist;
}

const char* Parser::re_packet_msg(char* buf, int cli_num){//����Ŷ
	char temp[512] = "";
	for (int i = 0; i < ((char)sizeof(buf) - restart); i++){
		temp[i] = buf[restart + i];
	}

	sprintf_s(rebuf, "%d %s", cli_num, temp);

	return rebuf;
}