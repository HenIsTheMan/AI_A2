#ifndef App_H
#define App_H

#include <mutex>
#include <thread>

#include "Scene.h"
#include "timer.h"

class App{
public:
	~App() = default;

	static App& GetInstance(){
		static App app;
		return app;
	}

	void Init();
	void Run();
	void Exit();

	static void QuickRender();

	static bool Key(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static void GetCursorPos(double* xpos, double* ypos);
private:
	App() = default;

	StopWatch im_Timer = StopWatch();

	std::mutex myMutex;
	
	static IScene* im_Scene;

	void Update();
	void Render();
};

#endif