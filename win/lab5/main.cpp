#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>
#include <string>
#include <iostream>

using namespace std;

struct asn_struct {
	HANDLE file;
	char *buffer;
	int maxsize;
	int realsize;
};

DWORD WINAPI writer(LPVOID lpParam);
HANDLE sem_read;
HANDLE sem_write;
HANDLE sem_end;

#define MAX_SIZE 100
char buff[MAX_SIZE];
int realsize;
string directory = "files/";

int main() {
	sem_read = CreateSemaphore(NULL, 0, 1, "read");
	sem_write = CreateSemaphore(NULL, 0, 1, "write");
	sem_end = CreateSemaphore(NULL, 0, 1, "end");
	HANDLE thread;
	thread = CreateThread(NULL, 0, writer, 0, 0, 0);


	HMODULE lib = LoadLibrary("lab5lib.dll");
	if (!lib) {
		printf("Error");
		exit(1);
	}
	void(*asn_read)(struct asn_struct *) = (void(*)(struct asn_struct *))GetProcAddress(lib, "asn_read");

	struct asn_struct * asn = (struct asn_struct *)malloc(sizeof(struct asn_struct));
	asn->buffer = (char*)calloc(100, sizeof(char));
	asn->maxsize = 100;
	char path[100];
	
	ReleaseSemaphore(sem_read, 1, NULL);

	for (int i = 0; i < 10; i++) {
		sprintf(path, "files/%d.txt", i);
		asn->file = CreateFile(
			path,				    //fileName
			GENERIC_READ,			//DesiredAccess (Read)
			0,						//shareMode (Prevents other processes from opening a file)
			NULL,					//secirityAttributes
			OPEN_EXISTING,			//CreationDisposition (open existing file)
			FILE_FLAG_OVERLAPPED,   //FlagsAndAttributes (The file is being opened or created for asynchronous I/O)
			NULL);					//TemplateFile (Can be NULL. When opening an existing file, CreateFile ignores this parameter.)
		if (asn->file == 0) {
			return 0;
		}		
		WaitForSingleObject(sem_read, INFINITE);

		asn_read(asn);
		strcpy(buff,asn->buffer);
		realsize = asn->realsize;
		ReleaseSemaphore(sem_write, 1, NULL);
	}
	ReleaseSemaphore(sem_end, 1, NULL);
	WaitForSingleObject(sem_read, INFINITE);
	CloseHandle(sem_read);
	CloseHandle(sem_write);
	CloseHandle(sem_end);
	CloseHandle(thread);
	FreeLibrary(lib);
	return 0;
}

DWORD WINAPI writer(LPVOID lpParam) {
	HANDLE sem_read;
	HANDLE sem_write;
	HANDLE sem_end;
	sem_read = OpenSemaphore(SEMAPHORE_ALL_ACCESS, SYNCHRONIZE, "read");
	sem_write = OpenSemaphore(SEMAPHORE_ALL_ACCESS, SYNCHRONIZE, "write");
	sem_end = OpenSemaphore(SEMAPHORE_ALL_ACCESS, SYNCHRONIZE, "end");


	HMODULE lib = LoadLibrary("lab5lib.dll");
	if (!lib) {
		printf("Error");
		exit(1);
	}
	void(*asn_write)(struct asn_struct *) = (void(*)(struct asn_struct *))GetProcAddress(lib, "asn_write");
	
	struct asn_struct * asn = (struct asn_struct *)malloc(sizeof(struct asn_struct));
	asn->buffer = (char*)calloc(100, sizeof(char));
	asn->maxsize = 100;
	asn->file = CreateFile("RESULT.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
	if (!asn->file) {
		printf("CreateFile error\n");
		exit(1);
	}

	while (1) {
		WaitForSingleObject(sem_write, INFINITE);

		asn->buffer = (char*)calloc(100, sizeof(char));
		strncpy(asn->buffer, buff, realsize);
		asn->maxsize = realsize;
		asn_write(asn);

		if (WaitForSingleObject(sem_end, 1) == WAIT_OBJECT_0) {
			break;
		}
		ReleaseSemaphore(sem_read, 1, NULL);
	}
	printf("The result is saved to a file \"RESULT\"\n");
	CloseHandle(sem_read);
	CloseHandle(sem_write);
	CloseHandle(sem_end);
	FreeLibrary(lib);
	ReleaseSemaphore(sem_write, 1, NULL);
	return 0;
}