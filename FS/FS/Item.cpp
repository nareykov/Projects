#include "Item.h"

void Item::toFile(Item item, string path) {
	ofstream out(path, ios::binary | ios::out | ios::trunc); //Открываем файл в двоичном режиме для записи
	toFileRecursive(item, out);
	out.close(); //Закрываем файл
}

void Item::toFileRecursive(Item item, ofstream & out) {
	if (item.childs.size() == 0) {
		int temp = item.childs.size();
		out.write((char*)item.name, NAME_SIZE);
		out.write((char*)&temp, sizeof(int));
		return;
	}
	else {
		out.write((char*)item.name, NAME_SIZE);
		int temp = item.childs.size();
		out.write((char*)&temp, sizeof(int));
		for (int i = 0; i < item.childs.size(); i++) {
			toFileRecursive(* item.childs[i], out);
		}
	}
}

Item * Item::fromFile(string path)
{
	ifstream in(path, ios::in | ios::binary);
	Item * item = fromFileRecursive(in, NULL);
	in.close();
	return item;
}

Item * Item::fromFileRecursive(ifstream & in, Item * parent)
{
	char name[NAME_SIZE];
	in.read((char *)&name, NAME_SIZE);
	int size;
	in.read((char *)&size,sizeof(int));
	cout << "read " << name << endl << " size " << size << endl;
	if (parent) {
		cout << "parent " << parent->name << endl;
	}
	Item * item = new Item(name, parent);

	if (size != 0) {
		for (int i = 0; i < size; i++) {
			item->addChild(fromFileRecursive(in, item));
		}
	}

	return item;
}

void Item::addChild(Item * child)
{
	this->size++;
	this->childs.push_back(child);
}
