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

#ifdef _WIN32
		UserEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, UserEventName);
		ATmEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, ATmEventName);


		hNamedPipe = CreateFile(PipeName, GENERIC_READ | GENERIC_WRITE,
			0, NULL, OPEN_EXISTING, 0, NULL);


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