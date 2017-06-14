#include "Item.h"

void Item::toFile(Item item, string path) {
	ofstream out(path, ios::binary | ios::out | ios::trunc); //Открываем файл в двоичном режиме для записи
	toFileRecursive(item, out);
	out.close(); //Закрываем файл
}

void Item::toFileRecursive(Item item, ofstream & out) {
	out.write((char*)&(item.type), sizeof(int));
	if (item.type == 1) {
		out.write((char*)item.name, NAME_SIZE);
		out.write((char*)&(item.size), sizeof(int));
		out.write((char*)item.content, item.size);
	} else {
		if (item.size == 0) {
			out.write((char*)item.name, NAME_SIZE);
			out.write((char*)&(item.size), sizeof(int));
			return;
		}
		else {
			out.write((char*)item.name, NAME_SIZE);
			out.write((char*)&(item.size), sizeof(int));
			for (int i = 0; i < item.size; i++) {
				toFileRecursive(* item.childs[i], out);
			}
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
	Item * item;
	int type;
	in.read((char *)&type, sizeof(int));
	if (type) {
		char name[NAME_SIZE];
		in.read((char *)&name, NAME_SIZE);
		int size;
		in.read((char *)&size, sizeof(int));
		char * content;
		in.read((char *)&content, size);
		item = new Item(name, parent, size, content);
	}
	else {
		char name[NAME_SIZE];
		in.read((char *)&name, NAME_SIZE);
		int size;
		in.read((char *)&size,sizeof(int));
		item = new Item(name, parent);

		if (size != 0) {
			for (int i = 0; i < size; i++) {
				item->addChild(fromFileRecursive(in, item));
			}
		}
	}
	

	return item;
}

void Item::addChild(Item * child)
{
	this->size++;
	this->childs.push_back(child);
}
