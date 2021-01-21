#ifndef App_H
#define App_H

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

	void QuickRender();

	static bool Key(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static void GetCursorPos(double* xpos, double* ypos);
private:
	App() = default;

	IScene* im_Scene = nullptr;
	StopWatch im_Timer = StopWatch();
};

#endif