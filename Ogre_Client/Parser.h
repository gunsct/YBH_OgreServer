#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

class Parser{
private:
	int pn, on, rg;
	float xp, yp, zp;
	
public:
	Parser();
	~Parser();


	const char* packeting(int pn, int on, int regi, float x, float y, float z);
	void parsing_msg(char* buf);

	int get_pn();
	int get_on();
	int get_rg();
	float get_xp();
	float get_yp();
	float get_zp();
};