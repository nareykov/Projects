#ifndef PAYPHONE_H
#define PAYPHONE_H

#include <iostream>
using namespace std;
#include <string>
#include <sstream>

class Payphone
{
private:
	int id;
	int money = 0;

public:
	Payphone(int id);
	string callRequest(string);
	int getId();
	string addMoney(string);
	int getMoney();
	void setMoney(int money);
};

#endif