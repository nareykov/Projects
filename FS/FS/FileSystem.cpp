#include "FileSystem.h"

void FileSystem::back()
{
	if (currItem->parent) {
		currItem = currItem->parent;
	}
}

void FileSystem::go_to()
{
	char name[NAME_SIZE];
	cout << "������� �������� ����������: ";
	cin >> name;
	for (int i = 0; i < currItem->size; i++) {
		if (!strcmp(name, currItem->childs[i]->name)) {
			currItem = currItem->childs[i];
			return;
		}
	}
	cout << "���������� �� �������" << endl;
}

void FileSystem::add()
{
	int type;
	cout << "��������� ��� (0 - �����, 1 - ����): ";
	cin >>type;
	char name[NAME_SIZE];
	cout << "������� �������� ��������: ";
	cin >> name;
	if (type) {
		int size;
		cout << "������� ������ �����������: ";
		cin >> size;
		char * content = (char *)malloc(size); 
		cout << "�������: ";
		cin >> content;
		currItem->addChild(new Item(name, currItem, size, content));
	}
	else {
		currItem->addChild(new Item(name, currItem));
	}
}

void FileSystem::del()
{
	char name[NAME_SIZE];
	cout << "������� �������� ��������: ";
	cin >> name;
	for (int i = 0; i < currItem->size; i++) {
		if (!strcmp(name, currItem->childs[i]->name)) {
			currItem->size--;
			currItem->childs.erase(currItem->childs.begin() + i);
			return;
		}
	}
}

void FileSystem::rename()
{
	char name[NAME_SIZE];
	char newname[NAME_SIZE];
	cout << "������� �������� ��������: ";
	cin >> name;
	cout << "������� ����� ���: ";
	cin >> newname;
	for (int i = 0; i < currItem->size; i++) {
		if (!strcmp(name, currItem->childs[i]->name)) {
			strcpy(currItem->childs[i]->name, newname);
			return;
		}
	}
}

void FileSystem::replace()
{
	char name[NAME_SIZE];
	cout << "������� �������� ��������: ";
	cin >> name;
	for (int i = 0; i < currItem->size; i++) {
		if (!strcmp(name, currItem->childs[i]->name)) {
			buff = currItem->childs[i];
			currItem->childs.erase(currItem->childs.begin() + i);
			currItem->size--;
			return;
		}
	}
}

void FileSystem::paste()
{
	if (buff) {
		currItem->addChild(buff);
		buff = NULL;
	}
}

void FileSystem::showfile()
{
	if (currItem->type) {
		cout << currItem->content << endl;
	}
	else {
		for (int i = 0; i < currItem->childs.size(); i++) {
			cout << currItem->childs[i]->name << endl;
		}
	}
}