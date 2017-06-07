#pragma once
#include "Item.h"

class FileSystem {
	

public:
	Item * root;
	Item * currItem;
	Item * buff;
	
	FileSystem(string path) {
		root = root->fromFile(path);
		currItem = root;
	}

	void back();
	void go_to();
	void add();
	void del();
	void rename();
	void replace();
	void paste();
};