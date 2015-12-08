//동기화를 위해 크리티컬 섹션을 쓰려함 성능상 뮤텍스보다 좋은거로 알고있음
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
