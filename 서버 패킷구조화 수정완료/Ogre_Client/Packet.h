#pragma once
#include <time.h>

enum error_list { EMPTY_BUF = 100, };
enum packetType { PT_CLIREGI = 1000, PT_START, PT_END, PT_SETPOS, PT_NONE };

class packetHeader
{
public:
	packetHeader()
	{
		m_Type = PT_NONE;
		m_uiRemainsize = 0;
	}
	packetHeader(enum packetType type , unsigned int size )
	{
		m_Type = type;
		m_uiRemainsize = size - sizeof(packetHeader);
	}

	enum packetType m_Type;
	unsigned int m_uiRemainsize;
};

class packetClientRegi : public packetHeader
{
public:
	packetClientRegi(): packetHeader() {}
	packetClientRegi(int num, bool cr) : packetHeader(PT_CLIREGI, sizeof(packetClientRegi))
	{
		cli_num = num;
		check_regi = cr;
	}

	bool check_regi;//Ŭ�󿡼��� true���� false������ ��ǥ���� ������ �Ǻ��ϱ⿡ ������ Ŭ�� �����ϰԾ�
	int cli_num;//���Ϳ� ��� ��ȣ�� �ٽ� ��ȯ���ִ°Ŵ�
};

class packetStart : public packetHeader
{
public:
	packetStart() : packetHeader() {}
	packetStart(int num) : packetHeader(PT_START, sizeof(packetStart))
	{
		fugitive_num = num;
	}
	int fugitive_num;//���Ϳ� ������ ������ ���� �������� ������ ��ȣ�� �ְ� �̰� ���� Ŭ����� �� Ŭ�� �i���� �ȴ�
};

class packetEnd : public packetHeader
{
public:
	packetEnd() : packetHeader() {}
	packetEnd(int num, clock_t ct) : packetHeader(PT_END, sizeof(packetEnd))
	{
		catcher_num = num;
		cTime = ct;
	}
	int catcher_num;//������� ��ȣ
	double cTime;// ��µ� �ɸ� �ð�
};

class packetSetPos : public packetHeader
{
public:
	packetSetPos() : packetHeader() {}
	packetSetPos(int num, float x, float y, float z) : packetHeader(PT_SETPOS, sizeof(packetSetPos))
	{
		xPos = x;
		yPos = y;
		zPos = z;
		cli_num = num;
	}

	float xPos, yPos, zPos;
	int cli_num;
};