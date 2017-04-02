#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <limits>
using namespace std;

#ifdef _WIN32
#include <process.h>
#include <Windows.h>
#include <string>
#endif

#include <sstream>
#include "payphone.h"

class Process
{
private:
	Process() {}

#ifdef _WIN32
	static HANDLE hNamedPipe;
	static LPSTR PipeName;

	static HANDLE UserEvent;
	static LPSTR UserEventName;

	static HANDLE ATmEvent;
	static LPSTR ATmEventName;
#endif

public:
	static void START(int argc, char *argv[]);
};

#endif