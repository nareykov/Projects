#include "process.h"

#ifdef _WIN32
LPSTR Process::PipeName = "\\\\.\\pipe\\$SecurityATmLine$";
HANDLE Process::hNamedPipe;
LPSTR Process::SecondPipeName = "\\\\.\\pipe\\SecondPipe";
HANDLE Process::hSecondPipe;
HANDLE Process::UserEvent;
LPSTR Process::UserEventName = "User";
HANDLE Process::PhoneEvent;
LPSTR Process::PhoneEventName = "Phone";
HANDLE Process::ConnectEvent;
LPSTR Process::ConnectEventName = "Connect";
HANDLE Process::ConnectBackEvent;
LPSTR Process::ConnectBackEventName = "ConnectBack";
HANDLE Process::hSemaphore;
LPSTR Process::SemaphoreName = "Semaphore";
HANDLE Process::hSecondSemaphore;
LPSTR Process::SecondSemaphoreName = "SecondSemaphore";
#endif

#ifdef linux
int Process::PipeDescript;
char Process::PipeName[21] = "/tmp/SecurityATmLine";
bool Process::User = false;
bool Process::Phone = false;
sem_t* Process::hSemaphore;
pid_t Process::pid;
struct sigaction Process::act;
#endif

void Process::START(int argc, char *argv[])
{

#ifdef _WIN32
	UserEvent = CreateEvent(NULL, TRUE, FALSE, UserEventName);
	PhoneEvent = CreateEvent(NULL, TRUE, FALSE, PhoneEventName);

	hNamedPipe = CreateNamedPipe(PipeName, PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		512, 512, 5000, NULL);

	std::thread t(threadFunction);

	hSemaphore = CreateSemaphore(NULL, 1, 1, SemaphoreName);

	if (hSemaphore == NULL)
	{
		cout << "error create semaphore" << endl;
	}

	system("title Payphone");
#endif

#ifdef linux

	sem_unlink("Semaphore");

	hSemaphore = sem_open("Semaphore", O_CREAT, 0777, 1);

	unlink(PipeName);
	mkfifo(PipeName, 0777);

	PipeDescript = open(PipeName, O_RDWR);

	memset(&act, 0, sizeof(act));
	act.sa_sigaction = user_signal;
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	act.sa_mask = set;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, 0);

	system("clear");
#endif

	Payphone *phone = new Payphone(1);


	while (true)
	{

#ifdef _WIN32
		ConnectNamedPipe(hNamedPipe, NULL);

		WaitForSingleObject(UserEvent, INFINITE);
		ResetEvent(UserEvent);
#endif

#ifdef linux
		///////////////////////////////
		while (!User);
		User = false;
#endif

		char buf[255];
		int b;

#ifdef _WIN32
		ReadFile(hNamedPipe, reinterpret_cast<char*>(&b), sizeof(int), NULL, NULL);
		ReadFile(hNamedPipe, buf, b, NULL, NULL);
#endif

#ifdef linux
		read(PipeDescript, reinterpret_cast<char*>(&b), sizeof(int));
		read(PipeDescript, buf, b);
#endif

		char t = '\0';
		buf[b] = t;


		if (!strncmp(buf, "local", 5) || !strncmp(buf, "mezh", 4))
		{
			string answer = phone->callRequest(string(buf));

			int i = answer.size();

#ifdef _WIN32
			WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
			WriteFile(hNamedPipe, answer.c_str(), i, NULL, NULL);

			SetEvent(PhoneEvent);
#endif

#ifdef linux
			write(PipeDescript, reinterpret_cast<char*>(&i), sizeof(int));
			write(PipeDescript, answer.c_str(), i);

			kill(pid, SIGUSR2);
#endif
		}
		else if (!strncmp(buf, "Shutdown", 8))
		{
#ifdef _WIN32
			DisconnectNamedPipe(hNamedPipe);
			SetEvent(PhoneEvent);
			continue;
#endif

#ifdef linux
			close(PipeDescript);

#endif
		}
		else if (!strncmp(buf, "add", 3))
		{
			string answer = phone->addMoney(string(buf));

			int i = answer.size();

#ifdef _WIN32
			WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
			WriteFile(hNamedPipe, answer.c_str(), i, NULL, NULL);

			SetEvent(PhoneEvent);
#endif

#ifdef linux
			write(PipeDescript, reinterpret_cast<char*>(&i), sizeof(int));
			write(PipeDescript, answer.c_str(), i);

			kill(pid, SIGUSR2);
#endif

		}
		else if (!strncmp(buf, "State", 5))
		{
			stringstream temp;
			temp << phone->getMoney();

			string answer = "You have ";
			answer.append(temp.str());
			answer.append("$");

			int i = answer.size();

#ifdef _WIN32
			WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
			WriteFile(hNamedPipe, answer.c_str(), i, NULL, NULL);

			SetEvent(PhoneEvent);
#endif

#ifdef linux
			write(PipeDescript, reinterpret_cast<char*>(&i), sizeof(int));
			write(PipeDescript, answer.c_str(), i);

			kill(pid, SIGUSR2);
#endif
		}
	}
}

#ifdef linux

void Process::user_signal(int s, siginfo_t *info, void *context)
{
	User = true;
	pid = info->si_pid;
}
#endif

void Process::threadFunction()
{
	hSecondPipe = CreateNamedPipe(SecondPipeName, PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		512, 512, 5000, NULL);

	ConnectEvent = CreateEvent(NULL, TRUE, FALSE, ConnectEventName);
	ConnectBackEvent = CreateEvent(NULL, TRUE, FALSE, ConnectBackEventName);

	hSecondSemaphore = CreateSemaphore(NULL, 1, 1, SecondSemaphoreName);

	vector<string> myVector;

	if (hSecondSemaphore == NULL)
	{
		cout << "error create second semaphore" << endl;
	}

	cout << "Waiting for users";

	while (true)
	{

#ifdef _WIN32
		ConnectNamedPipe(hSecondPipe, NULL);

		WaitForSingleObject(ConnectEvent, INFINITE);
		ResetEvent(ConnectEvent);
#endif

		char buf[255];
		int b;

#ifdef _WIN32
		ReadFile(hSecondPipe, reinterpret_cast<char*>(&b), sizeof(int), NULL, NULL);
		ReadFile(hSecondPipe, buf, b, NULL, NULL);
#endif

		char t = '\0';
		buf[b] = t;

		string te = string(buf);

		stringstream temp(te);

		string type;
		temp >> type;

		string Name;
		temp >> Name;

		cout << type << endl << Name << endl;


		if (!strncmp(buf, "connected", 9))
		{
			myVector.push_back(Name);
			printUsers(myVector);
		} else if (!strncmp(buf, "disconnected", 12)) 
		{
			for (int i = 0; i < myVector.size(); i++) {
				if (myVector[i] == Name) {
					myVector.erase(myVector.begin() + i);
				}
			}
			printUsers(myVector);
		}
		DisconnectNamedPipe(hSecondPipe);
		SetEvent(ConnectBackEvent);
	}

}

void Process::printUsers(vector<string> myVector)
{
	system("cls||clear");
	if (myVector.size() == 0)
	{
		cout << "Waiting for users";
		return;
	}
	
	for (int i = 0; i < myVector.size(); i++)
	{
		cout << i << ") " << myVector[i] << endl;
	}
}
