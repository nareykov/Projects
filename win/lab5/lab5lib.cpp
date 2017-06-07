#include <Windows.h>
#include <stdio.h>



struct asn_struct {
	HANDLE file;
	char *buffer;
	int maxsize;
	int realsize;
};

extern "C" __declspec(dllexport)void asn_read(struct asn_struct * asn) {

	OVERLAPPED overlapped = { 0 };

	if (ReadFile(asn->file, asn->buffer, asn->maxsize,(LPDWORD)&asn->realsize, (LPOVERLAPPED)&overlapped)) { //ReadFile != 0 - error
		printf("ReadFile error\n");
		return;
	}
	
	if (!GetOverlappedResult(asn->file, &overlapped, (LPDWORD)&asn->realsize, TRUE)) { //wait and check
		printf("Error\n");
		exit(1);
	}
}

extern "C" __declspec(dllexport)void asn_write(struct asn_struct * asn) {

	OVERLAPPED overlapped = { 0 };
	overlapped.Offset = SetFilePointer(asn->file, 0, NULL, FILE_END);;

	if (WriteFile(asn->file, asn->buffer, asn->maxsize, NULL, &overlapped) && GetLastError() == ERROR_IO_PENDING) {
		printf("Writing to file error\n");
		exit(1);
	}

	if (!GetOverlappedResult(asn->file, &overlapped, (LPDWORD)&asn->realsize, TRUE)) {
		printf("error\n");
		exit(1);
	}
}