//패킷 분석하는부분 클라이언트쪽에서 짜야할거같다
//아마 나는 패킷을 클라넘버,최초 생성여부,좌표 로 구성하고 이걸 특수문자나 띄어쓰기 단위로 따로 따로 분석해서 자료형에 맞춰 하지않을까 한다
//패킷 맨처음에 넘버랑 생성부분 세자리만 체크하면 됨 서버는 이거 3개만 필요 클라나 복잡하게 짜야지..

#include"Socket_client.h"

class Parser{
private:
	char buf[4];//넘버2자리 구분문자 1자리 생성1자리
	int cli_num;
	int regist;

public:
	Parser();
	~Parser();
	void num_regist(Socket_Cli SC);
};