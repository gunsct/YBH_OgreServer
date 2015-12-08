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
	packetClientRegi(){}
	packetClientRegi(int num, bool cr) : packetHeader(PT_CLIREGI, sizeof(packetClientRegi))
	{
		cli_num = num;
		check_regi = cr;
	}

	bool check_regi;//클라에서는 true인지 false인지로 좌표전달 시작을 판별하기에 서버나 클라나 동일하게씀
	int cli_num;//벡터에 담긴 번호를 다시 반환해주는거다
};

class packetStart : public packetHeader
{
public:
	packetStart(){}
	packetStart(int num) : packetHeader(PT_START, sizeof(packetStart))
	{
		fugitive_num = num;
	}
	int fugitive_num;//벡터에 일정량 유저가 차면 랜덤으로 도망자 번호를 주고 이걸 받은 클라들은 그 클라만 쫒으면 된다
};

class packetEnd : public packetHeader
{
public:
	packetEnd(){}
	packetEnd(int num, clock_t ct) : packetHeader(PT_END, sizeof(packetEnd))
	{
		catcher_num = num;
		cTime = ct;
	}
	int catcher_num;//잡은사람 번호
	clock_t cTime;// 잡는데 걸린 시간
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