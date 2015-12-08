//����ȭ�� ���� ũ��Ƽ�� ������ ������ ���ɻ� ���ؽ����� �����ŷ� �˰�����
//#pragma once
#include <Windows.h>

class Critical
{
	
	CRITICAL_SECTION cs;
public:
	Critical(void){ InitializeCriticalSection(&cs); }

	~Critical(void){ DeleteCriticalSection(&cs); }

	void Enter(){ EnterCriticalSection(&cs); }
	void Leave(){ LeaveCriticalSection(&cs); }
};
