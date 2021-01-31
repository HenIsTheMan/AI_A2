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

	static bool Key(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static void GetCursorPos(double* xpos, double* ypos);
private:
	App() = default;

	double updateDt = 0.0f;
	double renderDt = 0.0f;
	StopWatch im_UpdateTimer = StopWatch();
	StopWatch im_RenderTimer = StopWatch();
	
	static IScene* im_Scene;

	void Render();
};

#endif