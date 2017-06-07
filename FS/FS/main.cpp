#include "Item.h"
#include "FileSystem.h"


int main() {

	setlocale(LC_ALL, "RUS");
	FileSystem * fs = new FileSystem("files");

	int command;
	while (true)
	{
		system("cls");
		cout << "������� ���������� :" << fs->currItem->name << endl;
		cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
		for (int i = 0; i < fs->currItem->childs.size(); i++) {
			cout << fs->currItem->childs[i]->name << endl;
		}
		cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
		cout << "�������� �������:" << endl;
		cout << "1 - �����" << endl;
		cout << "2 - ������� � ..." << endl;
		cout << "3 - ��������" << endl;
		cout << "4 - �������" << endl;
		cout << "5 - �������������" << endl;
		cout << "6 - �����������" << endl;
		cout << "7 - ��������" << endl;
		cout << "0 - �����" << endl;
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