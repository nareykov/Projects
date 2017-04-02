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
				}
				else if (!strncmp(buf, "Shutdown", 8))
				{
					return;

				}
				else if (!strncmp(buf, "add", 3))
				{
					string answer = phone->addMoney(string(buf));

					int i = answer.size();

					WriteFile(hNamedPipe, reinterpret_cast<char*>(&i), sizeof(int), NULL, NULL);
					WriteFile(hNamedPipe, answer.c_str(), i, NULL, NULL);

					SetEvent(ATmEvent);

				}
				else if (!strncmp(buf, "State", 5))
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
