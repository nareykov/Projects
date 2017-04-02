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


	UserEvent = CreateEvent(NULL, TRUE, FALSE, UserEventName);
	ATmEvent = CreateEvent(NULL, TRUE, FALSE, ATmEventName);

	hNamedPipe = CreateNamedPipe(PipeName, PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		512, 512, 5000, NULL);

	system("title Payphone");
#endif

			Payphone *phone = new Payphone(1);


			while (true)
			{
				ConnectNamedPipe(hNamedPipe, NULL);
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
					DisconnectNamedPipe(hNamedPipe);
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
