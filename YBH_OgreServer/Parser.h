//��Ŷ �м��ϴºκ� Ŭ���̾�Ʈ�ʿ��� ¥���ҰŰ���
//�Ƹ� ���� ��Ŷ�� Ŭ��ѹ�,���� ��������,��ǥ �� �����ϰ� �̰� Ư�����ڳ� ���� ������ ���� ���� �м��ؼ� �ڷ����� ���� ���������� �Ѵ�
//��Ŷ ��ó���� �ѹ��� �����κ� ���ڸ��� üũ�ϸ� �� ������ �̰� 3���� �ʿ� Ŭ�� �����ϰ� ¥����..
#include"Socket_client.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>

class Parser{
private:
	char rebuf[512];//�ѹ�2�ڸ� ���й��� 1�ڸ� ����1�ڸ�
	int restart;
	//int cli_num; �������� ������
	int regist;

public:
	Parser();
	~Parser();

	int parsing_msg(char* buf);
	const char* re_packet_msg(char* buf, int cli_num);
};

