#include "process.h"

#ifdef _WIN32
LPSTR Process::PipeName = "\\\\.\\pipe\\$SecurityATmLine$";
HANDLE Process::hNamedPipe;
HANDLE Process::UserEvent;
LPSTR Process::UserEventName = "User";
HANDLE Process::ATmEvent;
LPSTR Process::ATmEventName = "ATM";
#endif

using namespace std;


void Process::START(int argc, char *argv[])
{

	if (argc == 1)
	{
#ifdef _WIN32
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		GetStartupInfo(&si);

		CreateProcess(NULL, (LPSTR)("taksofon.exe start_payphone"), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

		UserEvent = CreateEvent(NULL, TRUE, FALSE, UserEventName);
		ATmEvent = CreateEvent(NULL, TRUE, FALSE, ATmEventName);

		hNamedPipe = CreateNamedPipe(PipeName, PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,
			512, 512, 5000, NULL);

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
			cout << "4. Shutdown system" << endl;

			int menu;

			while (!(cin >> menu) || menu < 1 || menu > 4)
			{
				cout << "Please input correct value!\n";
				cin.clear();
#ifdef _WIN32
				cin.ignore(cin.rdbuf()->in_avail());
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

				WaitForSingleObject(ATmEvent, INFINITE);
				ResetEvent(ATmEvent);
#endif
				char buf[255];
				int b;

#ifdef _WIN32
				ReadFile(hNamedPipe, reinterpret_cast<char*>(&b), sizeof(int), NULL, NULL);
				ReadFile(hNamedPipe, buf, b, NULL, NULL);
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

					WaitForSingleObject(ATmEvent, INFINITE);
					ResetEvent(ATmEvent);
#endif
					char buf[255];
					int b;

#ifdef _WIN32
					ReadFile(hNamedPipe, reinterpret_cast<char*>(&b), sizeof(int), NULL, NULL);
					ReadFile(hNamedPipe, buf, b, NULL, NULL);
#endif
					buf[b] = '\0';

					cout << buf << endl;
				}
#ifdef _WIN32
				system("pause");
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

				WaitForSingleObject(ATmEvent, INFINITE);
				ResetEvent(ATmEvent);
#endif
				char buf[255];
				int b;

#ifdef _WIN32
				ReadFile(hNamedPipe, reinterpret_cast<char*>(&b), sizeof(int), NULL, NULL);
				ReadFile(hNamedPipe, buf, b, NULL, NULL);
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

#endif
				return;
			}
			}
		}
	}
	else
		if (argc >= 2)
		{

#ifdef _WIN32
			UserEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, UserEventName);
			ATmEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, ATmEventName);


			hNamedPipe = CreateFile(PipeName, GENERIC_READ | GENERIC_WRITE,
				0, NULL, OPEN_EXISTING, 0, NULL);

			system("title Payphone");


#endif
			Payphone *phone = new Payphone(1);


			while (true)
			{
#ifdef _WIN32
				WaitForSingleObject(UserEvent, INFINITE);
				ResetEvent(UserEvent);
#endif
				char buf[255];
				int b;

#ifdef _WIN32
				ReadFile(hNamedPipe, reinterpret_cast<char*>(&b), sizeof(int), NULL, NULL);
				ReadFile(hNamedPipe, buf, b, NULL, NULL);
#endif
				char t = '\0';
				buf[b] = t;
				cout << buf << endl;


				if (!strncmp(buf, "local", 5) || !strncmp(buf, "mezh", 4))
				{
					string answer = phone->callRequest(string(buf));

					int i = answer.size();

#ifdef _WIN32
					WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
					WriteFile(hNamedPipe, answer.c_str(), i, NULL, NULL);

					SetEvent(ATmEvent);
#endif
				} else if (!strncmp(buf, "Shutdown", 8))
				{
					return;

				} else if (!strncmp(buf, "add", 3)) 
				{
					string answer = phone->addMoney(string(buf));

					int i = answer.size();

					WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
					WriteFile(hNamedPipe, answer.c_str(), i, NULL, NULL);

					SetEvent(ATmEvent);

				} else if (!strncmp(buf, "State", 5))
				{
					stringstream temp;
					temp << phone->getMoney();

					string answer = "You have ";
					answer.append(temp.str());
					answer.append("$");

					int i = answer.size();

					WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
					WriteFile(hNamedPipe, answer.c_str(), i, NULL, NULL);

					SetEvent(ATmEvent);
				}
			}
		}
}