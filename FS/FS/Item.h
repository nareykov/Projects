#pragma once
#include <vector>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <fstream>
#include <iostream>

#define NAME_SIZE 15

using namespace :: std;

class Item {
public:
	Item * parent;
	char name[NAME_SIZE];
	int size;
	vector<Item *> childs;

	Item(char * name) {
		strcpy(this->name, name);
		this->size = 0;
	}

	Item(char * name, Item * parent) {
		strcpy(this->name, name);
		this->size = 0;
		this->parent = parent;
	}

	void toFile(Item item, string path);
	void toFileRecursive(Item item, ofstream & out);
	Item * fromFile(string path);
	Item * fromFileRecursive(ifstream & in, Item * parent = NULL);
	void addChild(Item * child);
};
