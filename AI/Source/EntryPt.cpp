#include "App.h"

#include <crtdbg.h>
#include <thread>
#include <string>

#ifndef SLN_CONFIG
	#ifdef _DEBUG
		#define SLN_CONFIG (std::string)"Debug"
	#else
		#define SLN_CONFIG (std::string)"Release"
	#endif
#endif

#ifndef SLN_PLAT
	#ifdef _WIN64
		#define SLN_PLAT (std::string)"x64"
	#else
		#define SLN_PLAT (std::string)"x86"
	#endif
#endif

bool endLoop;

void MainProcess(){
	App& app = App::GetInstance();
	app.Init();
	app.Run();
	app.Exit();
}

int main(const int&, const char* const* const&){
	(void)_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	(void)SetWindowPos(
		GetConsoleWindow(),
		0,
		0,
		0,
		GetSystemMetrics(SM_CXFULLSCREEN),
		GetSystemMetrics(SM_CYFULLSCREEN),
		0
	);
	(void)ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	(void)SetLayeredWindowAttributes(GetConsoleWindow(), NULL, 230, LWA_ALPHA);
	(void)SetConsoleTitleA(((std::string)"Console Window (" + SLN_CONFIG + ' ' + SLN_PLAT + ')').c_str());

	PHANDLER_ROUTINE ConsoleEventHandler = [](const DWORD event)->BOOL{
		LPCWSTR msg;
		switch(event){
			case CTRL_C_EVENT:
				msg = L"Ctrl + C";
				break;
			case CTRL_BREAK_EVENT:
				msg = L"Ctrl + BREAK";
				break;
			case CTRL_CLOSE_EVENT:
				msg = L"Closing prog...";
				break;
			case CTRL_LOGOFF_EVENT:
			case CTRL_SHUTDOWN_EVENT:
				msg = L"User is logging off...";
				break;
			default:
				msg = L"???";
		}

		MessageBoxW(NULL, msg, L"Msg", MB_OK);
		return TRUE;
	};
	(void)SetConsoleCtrlHandler(ConsoleEventHandler, TRUE);

	std::thread worker(&MainProcess);

	while(!endLoop){
		if(App::Key(VK_ESCAPE)){
			endLoop = true;
			break;
		}
	}

	worker.join();
}