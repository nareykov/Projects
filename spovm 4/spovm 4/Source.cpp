//Лабораторная №4. Потоковый менеджер

#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <process.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <string>
using namespace std;

DWORD WINAPI MyThread(LPVOID lpParam);

struct Storage {					//Хранилище для данных о потоке
	int N.					//Время ожидания для вывода списка
};

class Thread_Manager {			//класс для работы с потоками, также служит для кроссплатформенности
private:
	int currentNumber = 1;		//Текущее количество потоков программы
	int delayTime;				//Время задержки для вывода
	int create;					//Время для создания процесса

	struct Storage *storageArray;	//динамический массив данных о потоках
	HANDLE *arrayThread;			//динамический массив потоков

	void createThread();			//Метод для создания нового потока

public:
	Thread_Manager(int a, int b) {	//В конструкторе передаем промежутки времени
		delayTime = a;
		create = b;
		CreateSemaphore(NULL, 1, 1, L"sem");
	}

	void start() {

		storageArray = new Storage;		//Создаем массив из одного элемента
		storageArray[currentNumber - 1].Name = currentNumber;//И заполняем его данными
		storageArray[currentNumber - 1].finalNumber = &currentNumber;
		storageArray[currentNumber - 1].delay = delayTime;

		arrayThread = new HANDLE;		//Создаем массив для хранения самих потоков программы
		arrayThread[currentNumber - 1] = CreateThread(NULL, 0, MyThread, &storageArray[currentNumber - 1], 0, 0);//Создаем сам поток

		char temp;
		long long int start = clock(); //Начинаем отсчет времени

		while (1) {
			if (_kbhit()) {				//Проверка буфера на нахождение там символа
				temp = _getch();		//Если плюсик, то создаем поток
				if (temp == '+') {
					createThread();
				}
				if (temp == '-' && currentNumber > 1) {	//Если минусик, то уничтожаем
					TerminateThread(arrayThread[currentNumber - 1], 0);

					currentNumber--;

					arrayThread = (HANDLE*)realloc(arrayThread, currentNumber * sizeof(HANDLE)); //Перевыделяем память
					storageArray = (struct Storage*)realloc(storageArray, currentNumber * sizeof(struct Storage));//И для данных о потоке
				}
			}

			if ((clock() - start) > create)	//Если время для создания нового потока прошло,
			{
				start = clock();			//То начинаем отсчет заново

				createThread();				//И создаем поток
			}
		}
	}
};

void Thread_Manager::createThread() {		//Реализация метода создания потока
	currentNumber++;						//Увеличиваем число потоков

	storageArray = (struct Storage*)realloc(storageArray, currentNumber * sizeof(struct Storage));

	storageArray[currentNumber - 1].Name = currentNumber;
	storageArray[currentNumber - 1].finalNumber = &currentNumber;
	storageArray[currentNumber - 1].delay = delayTime;

	arrayThread = (HANDLE*)realloc(arrayThread, currentNumber * sizeof(HANDLE));
	arrayThread[currentNumber - 1] = CreateThread(NULL, 0, MyThread, &storageArray[currentNumber - 1], 0, 0);
}

DWORD WINAPI MyThread(LPVOID lpParam) {
	struct Storage storage = *((struct Storage*)lpParam);	//переменная структуры для хранения данных о текущем потоке
	HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, L"sem");

	while (true)					//Потоки работают бесконечно
	{
		WaitForSingleObject(hSemaphore, INFINITE);

		if (storage.Name == 1)		//Если работаем с первым потоком, то ждем нужное время
			Sleep(storage.delay);

		//Вывод очереди потоков
		if (storage.Name == *(storage.finalNumber))				//Если мы последний поток,
			cout << "Thread " << storage.Name << " " << endl;		//Делаем перенос строки
		else
			cout << "Thread " << storage.Name << " ";				//Или же просто выводим имя потока

		ReleaseSemaphore(hSemaphore, 1, NULL);					//И ставим текущий поток в очередь
	}

	return 0;
}

int main() {
	setlocale(LC_ALL, "Russian");
	int a, b;
	cout << "Задержка вывода" << endl;
	cin >> a;
	cout << "Задержка создания" << endl;
	cin >> b;
	CreateSemaphore(NULL, 1, 1, L"sem");

	Thread_Manager manager(a, b);
	manager.start();

	return 0;
}