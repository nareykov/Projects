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
	cout << "Введите название директории: ";
	cin >> name;
	for (int i = 0; i < currItem->size; i++) {
		if (!strcmp(name, currItem->childs[i]->name)) {
			currItem = currItem->childs[i];
			return;
		}
	}
	cout << "Директория не найдена" << endl;
}

void FileSystem::add()
{
	char name[NAME_SIZE];
	cout << "Введите название элемента: ";
	cin >> name;
	currItem->addChild(new Item(name, currItem));
}

void FileSystem::del()
{
	char name[NAME_SIZE];
	cout << "Введите название элемента: ";
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
	cout << "Введите название элемента: ";
	cin >> name;
	cout << "Введите новое имя: ";
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
	cout << "Введите название элемента: ";
	cin >> name;
	for (int i = 0; i < currItem->size; i++) {
		if (!strcmp(name, currItem->childs[i]->name)) {
			currItem->childs.erase(currItem->childs.begin() + i);
			currItem->size--;
			buff = currItem->childs[i];
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
