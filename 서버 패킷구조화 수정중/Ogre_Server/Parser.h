//��Ŷ �м��ϴºκ� Ŭ���̾�Ʈ�ʿ��� ¥���ҰŰ���
//�Ƹ� ���� ��Ŷ�� Ŭ��ѹ�,���� ��������,��ǥ �� �����ϰ� �̰� Ư�����ڳ� ���� ������ ���� ���� �м��ؼ� �ڷ����� ���� ���������� �Ѵ�
//��Ŷ ��ó���� �ѹ��� �����κ� ���ڸ��� üũ�ϸ� �� ������ �̰� 3���� �ʿ� Ŭ�� �����ϰ� ¥����..
#include"Socket_client.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>

class Parser{
private:
	char rebuf[513];//����Ŷ�Ҷ� ���� ����
	int restart;//��Ŷ �缳���Ҷ� �̾���̱� �����Ұ�
	//int cli_num; �������� ������
	int regist;//���Ϳ� ���ʵ������ �Ǻ��Ҷ� ��Ŷ���� �� �޴°�

	//��Ŷ�� ������
	char player_num[1];//�޴���
	char other_num[1];//��������
	char regi[1];
	char xp[20];
	char yp[20];
	char zp[20];

public:
	Parser();
	~Parser();
	void init();
	int parsing_msg(char* buf);//��Ͽ��θ� ������
	const char* re_packet_msg(char* buf,int re_cli_num, int se_cli_num);//�޴»�� �����»�� �ѹ� ����Ŷ����
};

