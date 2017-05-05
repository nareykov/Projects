#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <limits>
#include <stdio.h>  
#include <vector>

using namespace std;

#ifdef _WIN32
#include <process.h>
#include <Windows.h>
#include <string>
#endif

#ifdef linux
#include <sys/sem.h>
#include <sys/types.h>
#include <fcntl.h>          
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/stat.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <string.h>
#endif	

#include <sstream>
#include "payphone.h"

class Process
{
private:
	Process() {}

#ifdef _WIN32
	static HANDLE hSemaphore;
	static LPSTR SemaphoreName;

	static HANDLE hSecondSemaphore;
	static LPSTR SecondSemaphoreName;

	static HANDLE hNamedPipe;
	static LPSTR PipeName;

	static HANDLE hSecondPipe;
	static LPSTR SecondPipeName;

	static HANDLE UserEvent;
	static LPSTR UserEventName;

	static HANDLE ConnectEvent;
	static LPSTR ConnectEventName;

	static HANDLE ConnectBackEvent;
	static LPSTR ConnectBackEventName;

	static HANDLE PhoneEvent;
	static LPSTR PhoneEventName;
#endif

#ifdef linux
	static int PipeDescript;
	static char PipeName[21];

	static sem_t* hSemaphore;

	static struct sigaction act;
	static pid_t pid;

	static bool User;
	static bool Phone;
#endif

	static void threadFunction();
	static void printUsers(vector<string>);

public:
	static void START(int argc, char *argv[]);
	

#ifdef linux
	static void user_signal(int s, siginfo_t *info, void *context);
#endif
};

#endif
