#include "payphone.h"

Payphone::Payphone(int id)
{
	this->id = id;
}

string Payphone::callRequest(string request)
{

	stringstream buf(request);

	string type;
	buf >> type;

	int min;
	buf >> min;

	int cost = 0;

	if (!type.compare("local"))
	{
		cost = min;
	}
	else
	{
		cost = min * 2;
	}

	if (cost <= getMoney())
	{
		setMoney(getMoney() - cost);
		return "Call";
	}
	else
	{
		return "Need more gold";
	}
}

int Payphone::getId()
{
	return this->id;
}

string Payphone::addMoney(string request)
{
	stringstream buf(request);

	string temp;
	buf >> temp;

	int money;
	buf >> money;
	this->money += money;

	return "Success";
}

int Payphone::getMoney()
{
	return this->money;
}

void Payphone::setMoney(int money)
{
	this->money = money;
}
