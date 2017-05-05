#include "process.h"

#ifdef _WIN32
string Process::Name;
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
//מבתÿגטעü סולאפמנ
#ifdef linux
int Process::PipeDescript;
char Process::PipeName[21] = "/tmp/SecurityATmLine";
bool Process::User = false;
bool Process::Phone = false;
sem_t* Process::hSemaphore;
#endif

void Process::START(int argc, char *argv[])
{
	cout << "Input your name: ";
	cin >> Name;
	cout << "Waiting";	

#ifdef _WIN32
		//////////////////////////////
		hSecondSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, SecondSemaphoreName);

		DWORD dwWaitSecondResult;
		dwWaitSecondResult = WaitForSingleObject(hSecondSemaphore, 0L);
		while (dwWaitSecondResult != WAIT_OBJECT_0)
		{
			dwWaitSecondResult = WaitForSingleObject(hSecondSemaphore, 1);
		}

		ConnectEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, ConnectEventName);
		ConnectBackEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, ConnectBackEventName);

		hSecondPipe = CreateFile(SecondPipeName, GENERIC_READ | GENERIC_WRITE,
			0, NULL, OPEN_EXISTING, 0, NULL);

		stringstream temp;
		temp << "connected ";
		temp << Name;

		string request = temp.str();

		int i = request.size();

		WriteFile(hSecondPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
		WriteFile(hSecondPipe, request.c_str(), i, NULL, NULL);

		SetEvent(ConnectEvent);

		WaitForSingleObject(ConnectBackEvent, INFINITE);
		ResetEvent(ConnectBackEvent);

		CloseHandle(hSecondPipe);
		ReleaseSemaphore(hSecondSemaphore, 1, NULL);
		CloseHandle(hSecondSemaphore);
		/////////////////////////////

		hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, SemaphoreName);
		DWORD dwWaitResult;
		dwWaitResult = WaitForSingleObject(hSemaphore, 0L);
		while (dwWaitResult != WAIT_OBJECT_0)
		{
			dwWaitResult = WaitForSingleObject(hSemaphore, 1);
		}

		UserEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, UserEventName);
		PhoneEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, PhoneEventName);


		hNamedPipe = CreateFile(PipeName, GENERIC_READ | GENERIC_WRITE,
			0, NULL, OPEN_EXISTING, 0, NULL);


#endif

#ifdef linux
		hSemaphore = sem_open("Semaphore", 0);
		sem_wait(hSemaphore);
		signal(SIGUSR2, phone_signal);

		PipeDescript = open(PipeName, O_RDWR);
		pid_t pid = get_pid();
#endif

		Client client = Client(1);

#ifdef _WIN32
		string comand("title Client");
		system(comand.c_str());
#endif

		while (true)
		{
			system("cls||clear");
			cout << "Account menu:" << endl;
			cout << "1. State" << endl;
			cout << "2. Call" << endl;
			cout << "3. Add money" << endl;
			cout << "4. Exit" << endl;

			int menu;

			while (!(cin >> menu) || menu < 1 || menu > 4)
			{
				cout << "Please input correct value!\n";
				cin.clear();
#ifdef _WIN32
				cin.ignore(cin.rdbuf()->in_avail());
#endif


#ifdef linux
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
#endif
			}

			switch (menu)
			{
			case 1:
			{
				system("cls||clear");

				string request = "State";

				int i = request.size();

#ifdef _WIN32
				WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
				WriteFile(hNamedPipe, request.c_str(), i, NULL, NULL);

				SetEvent(UserEvent);

				WaitForSingleObject(PhoneEvent, INFINITE);
				ResetEvent(PhoneEvent);
#endif

#ifdef linux
				write(PipeDescript, reinterpret_cast<char*>(&i), sizeof(int));

				write(PipeDescript, request.c_str(), i);

				kill(pid, SIGUSR1);

				while (!Phone);
				Phone = false;
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

				buf[b] = '\0';
				if (!strncmp(buf, "You have", 8))
				{
					cout << buf << endl;
				}
				else
				{
					cout << "Error" << endl;
				}

#ifdef _WIN32
				system("pause");
#endif 

#ifdef linux
				press_any_key();
#endif
				break;
			}
			case 2:
			{
				system("cls||clear");

				string request = client.call();

				if (request.empty())
					cout << "Canceled" << endl;
				else
				{
					int i = request.size();

#ifdef _WIN32
					WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
					WriteFile(hNamedPipe, request.c_str(), i, NULL, NULL);

					SetEvent(UserEvent);

					WaitForSingleObject(PhoneEvent, INFINITE);
					ResetEvent(PhoneEvent);
#endif

#ifdef linux
					write(PipeDescript, reinterpret_cast<char*>(&i), sizeof(int));
					write(PipeDescript, request.c_str(), i);

					kill(pid, SIGUSR1);

					while (!Phone);
					Phone = false;
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

					buf[b] = '\0';

					cout << buf << endl;
				}
#ifdef _WIN32
				system("pause");
#endif 

#ifdef linux
				press_any_key();
#endif

				break;
			}
			case 3: 
			{
				system("cls||clear");

				int money;

				cout << "Enter amount: ";

				while (!(cin >> money) || money < 1)
				{
					cout << "Please input correct value!\n";
					cin.clear();
#ifdef _WIN32
					cin.ignore(cin.rdbuf()->in_avail());
#endif

#ifdef linux
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
#endif
				}

				stringstream temp;
				temp << "add ";
				temp << money;

				string request = temp.str();

				int i = request.size();

#ifdef _WIN32
				WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
				WriteFile(hNamedPipe, request.c_str(), i, NULL, NULL);

				SetEvent(UserEvent);

				WaitForSingleObject(PhoneEvent, INFINITE);
				ResetEvent(PhoneEvent);
#endif

#ifdef linux
				write(PipeDescript, reinterpret_cast<char*>(&i), sizeof(int));

				write(PipeDescript, request.c_str(), i);

				kill(pid, SIGUSR1);

				while (!Phone);
				Phone = false;
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

				buf[b] = '\0';
				if (!strncmp(buf, "Success", 7))
				{
					cout << buf << endl;
				}
				else
				{
					cout << "Error" << endl;
				}

#ifdef _WIN32
				system("pause");
#endif 

#ifdef linux
				press_any_key();
#endif

				break;
			}
			case 4:
			{

				system("cls||clear");

				string request = "Shutdown";

				int i = request.size();

#ifdef _WIN32
				WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
				WriteFile(hNamedPipe, request.c_str(), i, NULL, NULL);

				SetEvent(UserEvent);

				WaitForSingleObject(PhoneEvent, INFINITE);
				ResetEvent(PhoneEvent);

				CloseHandle(hNamedPipe);
				ReleaseSemaphore(hSemaphore, 1, NULL);
				CloseHandle(hSemaphore);

				///////////////////////////////////
				hSecondSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, SecondSemaphoreName);
				dwWaitSecondResult = WaitForSingleObject(hSecondSemaphore, 0L);
				while (dwWaitSecondResult != WAIT_OBJECT_0)
				{
					dwWaitSecondResult = WaitForSingleObject(hSecondSemaphore, 1);
				}

				ConnectEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, ConnectEventName);
				ConnectBackEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, ConnectBackEventName);

				hSecondPipe = CreateFile(SecondPipeName, GENERIC_READ | GENERIC_WRITE,
					0, NULL, OPEN_EXISTING, 0, NULL);

				stringstream temp;
				temp << "disconnected ";
				temp << Name;

				request = temp.str();

				i = request.size();

				WriteFile(hSecondPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
				WriteFile(hSecondPipe, request.c_str(), i, NULL, NULL);

				SetEvent(ConnectEvent);

				WaitForSingleObject(ConnectBackEvent, INFINITE);
				ResetEvent(ConnectBackEvent);

				CloseHandle(hSecondPipe);
				ReleaseSemaphore(hSecondSemaphore, 1, NULL);
				CloseHandle(hSecondSemaphore);
				////////////////////////////////////////////////////
				return;
#endif

#ifdef linux
				write(PipeDescript, reinterpret_cast<char*>(&i), sizeof(int));
				write(PipeDescript, request.c_str(), i);

				kill(pid, SIGUSR1);

				//while (!Phone);
				//Phone = false;
				
				sem_post(hSemaphore);
				return;
				//sem_close(Semaphore);
#endif
			}
		}
	}
}


#ifdef linux

void Process::phone_signal(int s)
{
	Phone = true;
	signal(SIGUSR2, phone_signal);
}

int Process::get_pid()
{
	sleep(3);
	char buf[128] = "/bin/pidof payphone";
	char* p = &buf[0];


	FILE* f = popen(buf, "r");
	if (!f) {
		perror("popen():");
		return -1;
	}
	int i = 0;
	while (!feof(f)) {
		*p++ = fgetc(f);
		i++;
	}
	pid_t pid;
	buf[i] = '\0';
	pid = atoi(buf);

	return pid;
}

void Process::press_any_key()
{
	cout << "please press ENTER....\0";
	cin.get();
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
#endif
