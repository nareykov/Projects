#include "Item.h"
#include "FileSystem.h"


int main() {

	setlocale(LC_ALL, "RUS");
	FileSystem * fs = new FileSystem("files");

	int command;
	while (true)
	{
		system("cls");
		cout << "Текущая директория :" << fs->currItem->name << endl;
		cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
		for (int i = 0; i < fs->currItem->childs.size(); i++) {
			cout << fs->currItem->childs[i]->name << endl;
		}
		cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
		cout << "Выберите команду:" << endl;
		cout << "1 - Назад" << endl;
		cout << "2 - Перейти в ..." << endl;
		cout << "3 - Добавить" << endl;
		cout << "4 - Удалить" << endl;
		cout << "5 - Переименовать" << endl;
		cout << "6 - Переместить" << endl;
		cout << "7 - Вставить" << endl;
		cout << "0 - Выход" << endl;
		cin >> command;

		switch (command) {
		case 1: 
			fs->back();
			break;
		case 2:
			fs->go_to();
			break;
		case 3:
			fs->add();
			break;
		case 4:
			fs->del();
			break;
		case 5:
			fs->rename();
			break;
		case 6:
			fs->replace();
			break;
		case 7:
			fs->paste();
			break;
		case 0 :
			fs->root->toFile(*(fs->root), "files");
			exit(1);
			break;
		default:
			cout << " Unknown command" << endl;
			getchar();
		}
	}
	system("pause");
}