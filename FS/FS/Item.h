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
	int type; //0 - folder, 1 - file 
	Item * parent;
	char name[NAME_SIZE];
	int size;
	char * content;
	vector<Item *> childs;

	Item(char * name, Item * parent) {
		this->type = 0;
		strcpy(this->name, name);
		this->size = 0;
		this->parent = parent;
	}

	Item(char * name, Item * parent, int size, char * content) {
		this->type = 1;
		strcpy(this->name, name);
		this->size = size;
		this->parent = parent;
		this->content = (char *)malloc(size);
		strcpy(this->content, content);
	}

	void toFile(Item item, string path);
	void toFileRecursive(Item item, ofstream & out);
	Item * fromFile(string path);
	Item * fromFileRecursive(ifstream & in, Item * parent = NULL);
	void addChild(Item * child);
};
