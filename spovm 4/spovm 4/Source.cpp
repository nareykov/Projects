//������������ �4. ��������� ��������

#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <process.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <string>
using namespace std;

DWORD WINAPI MyThread(LPVOID lpParam);

struct Storage {					//��������� ��� ������ � ������
	int N.					//����� �������� ��� ������ ������
};

class Thread_Manager {			//����� ��� ������ � ��������, ����� ������ ��� ��������������������
private:
	int currentNumber = 1;		//������� ���������� ������� ���������
	int delayTime;				//����� �������� ��� ������
	int create;					//����� ��� �������� ��������

	struct Storage *storageArray;	//������������ ������ ������ � �������
	HANDLE *arrayThread;			//������������ ������ �������

	void createThread();			//����� ��� �������� ������ ������

public:
	Thread_Manager(int a, int b) {	//� ������������ �������� ���������� �������
		delayTime = a;
		create = b;
		CreateSemaphore(NULL, 1, 1, L"sem");
	}

	void start() {

		storageArray = new Storage;		//������� ������ �� ������ ��������
		storageArray[currentNumber - 1].Name = currentNumber;//� ��������� ��� �������
		storageArray[currentNumber - 1].finalNumber = &currentNumber;
		storageArray[currentNumber - 1].delay = delayTime;

		arrayThread = new HANDLE;		//������� ������ ��� �������� ����� ������� ���������
		arrayThread[currentNumber - 1] = CreateThread(NULL, 0, MyThread, &storageArray[currentNumber - 1], 0, 0);//������� ��� �����

		char temp;
		long long int start = clock(); //�������� ������ �������

		while (1) {
			if (_kbhit()) {				//�������� ������ �� ���������� ��� �������
				temp = _getch();		//���� ������, �� ������� �����
				if (temp == '+') {
					createThread();
				}
				if (temp == '-' && currentNumber > 1) {	//���� �������, �� ����������
					TerminateThread(arrayThread[currentNumber - 1], 0);

					currentNumber--;

					arrayThread = (HANDLE*)realloc(arrayThread, currentNumber * sizeof(HANDLE)); //������������ ������
					storageArray = (struct Storage*)realloc(storageArray, currentNumber * sizeof(struct Storage));//� ��� ������ � ������
				}
			}

			if ((clock() - start) > create)	//���� ����� ��� �������� ������ ������ ������,
			{
				start = clock();			//�� �������� ������ ������

				createThread();				//� ������� �����
			}
		}
	}
};

void Thread_Manager::createThread() {		//���������� ������ �������� ������
	currentNumber++;						//����������� ����� �������

	storageArray = (struct Storage*)realloc(storageArray, currentNumber * sizeof(struct Storage));

	storageArray[currentNumber - 1].Name = currentNumber;
	storageArray[currentNumber - 1].finalNumber = &currentNumber;
	storageArray[currentNumber - 1].delay = delayTime;

	arrayThread = (HANDLE*)realloc(arrayThread, currentNumber * sizeof(HANDLE));
	arrayThread[currentNumber - 1] = CreateThread(NULL, 0, MyThread, &storageArray[currentNumber - 1], 0, 0);
}

DWORD WINAPI MyThread(LPVOID lpParam) {
	struct Storage storage = *((struct Storage*)lpParam);	//���������� ��������� ��� �������� ������ � ������� ������
	HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, L"sem");

	while (true)					//������ �������� ����������
	{
		WaitForSingleObject(hSemaphore, INFINITE);

		if (storage.Name == 1)		//���� �������� � ������ �������, �� ���� ������ �����
			Sleep(storage.delay);

		//����� ������� �������
		if (storage.Name == *(storage.finalNumber))				//���� �� ��������� �����,
			cout << "Thread " << storage.Name << " " << endl;		//������ ������� ������
		else
			cout << "Thread " << storage.Name << " ";				//��� �� ������ ������� ��� ������

		ReleaseSemaphore(hSemaphore, 1, NULL);					//� ������ ������� ����� � �������
	}

	return 0;
}

int main() {
	setlocale(LC_ALL, "Russian");
	int a, b;
	cout << "�������� ������" << endl;
	cin >> a;
	cout << "�������� ��������" << endl;
	cin >> b;
	CreateSemaphore(NULL, 1, 1, L"sem");

	Thread_Manager manager(a, b);
	manager.start();

	return 0;
}