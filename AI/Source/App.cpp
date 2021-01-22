#include "App.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

static GLFWwindow* s_UpdateWindow = nullptr;
static GLFWwindow* s_RenderWindow = nullptr;
static const unsigned char FPS = 60;
static const float frameTime = 1000.0f / (float)FPS;

///Shld be members of App instead
double mouseScrollWheelYOffset;
int windowWidth;
int windowHeight;

extern bool endLoop;

IScene* App::im_Scene = nullptr;

static void error_callback(int error, const char* description){
	fputs(description, stderr);
	_fgetchar();
}

static void resize_callback(GLFWwindow* window, int w, int h){
	windowWidth = w;
	windowHeight = h;
}

bool App::Key(unsigned short key){
    return GetAsyncKeyState(key) & 0x8000;
}

bool App::IsMousePressed(unsigned short key){ //0 - Left, 1 - Right, 2 - Middle
	return glfwGetMouseButton(s_RenderWindow, key) != 0;
}

void App::GetCursorPos(double* xpos, double* ypos){
	glfwGetCursorPos(s_RenderWindow, xpos, ypos);
}

static void ScrollCallback(GLFWwindow*, double xOffset, double yOffset){
	mouseScrollWheelYOffset += yOffset;
}

void App::Init(){
	glfwSetErrorCallback(error_callback);

	if(!glfwInit()){
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	s_UpdateWindow = glfwCreateWindow(1, 1, "Update Window", nullptr, nullptr);
	glfwHideWindow(s_UpdateWindow);

	const GLFWvidmode* const& mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	s_RenderWindow = glfwCreateWindow(mode->width / 2, mode->height / 2, "App Window", nullptr, nullptr);
	glfwSetWindowPos(s_RenderWindow, mode->width / 4, mode->height / 4);
	glfwMaximizeWindow(s_RenderWindow);
	glfwShowWindow(s_RenderWindow);
	glfwGetWindowSize(s_RenderWindow, &windowWidth, &windowHeight);

	if(!s_RenderWindow){
		fprintf(stderr, "Failed to open render window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(s_RenderWindow);

	glfwSetWindowSizeCallback(s_RenderWindow, resize_callback);
	glfwSetScrollCallback(s_RenderWindow, ScrollCallback);

	glewExperimental = true; //Needed for core profile
	GLenum err = glewInit();
	if(err != GLEW_OK){
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	im_Scene = new Scene();
	im_Scene->Init();

	glfwSwapInterval(0); //Disable VSync
}

void App::Render(){
	glfwMakeContextCurrent(s_RenderWindow);

	//Timer??
	while(!endLoop){
		if(glfwWindowShouldClose(s_RenderWindow) || Key(VK_ESCAPE)){
			endLoop = true;
		}

		if(glfwGetWindowAttrib(s_RenderWindow, GLFW_VISIBLE)){
			glViewport(0, 0, windowWidth, windowHeight);
			im_Scene->Render();
		}

		glfwSwapBuffers(s_RenderWindow);
	}
}

void App::Run(){
	glfwMakeContextCurrent(NULL);

	std::thread renderThread(&App::Render, this);

	glfwMakeContextCurrent(s_UpdateWindow);

	static bool isF3 = false;
	static bool isF1 = false;

	im_Timer.startTimer();
	while(!endLoop){
		if(glfwWindowShouldClose(s_UpdateWindow) || Key(VK_ESCAPE)){
			endLoop = true;
		}

		im_Scene->Update(im_Timer.getElapsedTime());

		/*if(!isF3 && Key(VK_F3)){
			glfwGetWindowAttrib(s_UpdateWindow, GLFW_VISIBLE) ? glfwHideWindow(s_UpdateWindow) : glfwShowWindow(s_UpdateWindow);
			isF3 = true;
		} else if(isF3 && !Key(VK_F3)){
			isF3 = false;
		}
		if(!isF1 && Key(VK_F1)){
			const GLFWvidmode* const& mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwSetWindowMonitor(s_UpdateWindow, glfwGetWindowMonitor(s_UpdateWindow) ? nullptr : glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
			isF1 = true;
		} else if(isF1 && !Key(VK_F1)){
			isF1 = false;
		}*/

		glfwPollEvents();
	}

	renderThread.join();
}

void App::Exit(){
	if(im_Scene){
		delete im_Scene;
		im_Scene = nullptr;
	}

	glfwDestroyWindow(s_UpdateWindow);
	glfwDestroyWindow(s_RenderWindow);
	glfwTerminate();
}

void App::QuickRender(){
	im_Scene->Render();
	glfwSwapBuffers(s_RenderWindow);
	glfwPollEvents();
}