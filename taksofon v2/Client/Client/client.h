#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <limits>
using namespace std;
#include <string>
#include <sstream>

class Client
{
private:
	int id;
	int money;
	int frozen;

public:
	Client();
	Client(int id);
	string call();
	int moneyAmount();
	void addMoney(int money);
	int getId();
};

#endif
