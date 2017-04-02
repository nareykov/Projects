#include "client.h"

Client::Client()
{
}

Client::Client(int id)
{
	this->id = id;
	this->money = 0;
	this->frozen = 0;
}

string Client::call()
{
	cout << "Choose type:\n";
	cout << "0.Exit\n";
	cout << "1.Local\n";
	cout << "2.Intercity\n";

	int menu;
	while (!(cin >> menu) || menu < 0 || menu > 2)
	{
		cout << "Please input correct value!\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	stringstream buf;

	switch (menu)
	{
	case 0: return "";
	case 1: buf << "local "; break;
	case 2: buf << "mezh "; break;
	}

	cout << "Enter the minutes: ";

	int minutes;
	while (!(cin >> minutes) || minutes < 0)
	{
		cout << "Please input correct value!\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	frozen = minutes;

	buf << minutes;

	return buf.str();
}

int Client::moneyAmount()
{
	return money;
}

int Client::getId()
{
	return this->id;
}

void Client::addMoney(int money)
{
	cout << "Added " << money << " $" << endl;
	this->money += money;
}