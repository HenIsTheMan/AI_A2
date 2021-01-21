#include "App.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "Scene.h"

GLFWwindow* im_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

///Shld be members of App instead
double mouseScrollWheelYOffset;
int windowWidth;
int windowHeight;

extern bool endLoop;

static void error_callback(int error, const char* description){
	fputs(description, stderr);
	_fgetchar();
}

static void resize_callback(GLFWwindow* window, int w, int h){
	windowWidth = w;
	windowHeight = h;
}

bool App::Key(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

bool App::IsMousePressed(unsigned short key) //0 - Left, 1 - Right, 2 - Middle
{
	return glfwGetMouseButton(im_window, key) != 0;
}

void App::GetCursorPos(double* xpos, double* ypos)
{
	glfwGetCursorPos(im_window, xpos, ypos);
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
	const GLFWvidmode* const& mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	im_window = glfwCreateWindow(mode->width / 2, mode->height / 2, "App Window", nullptr, nullptr);
	glfwSetWindowPos(im_window, mode->width / 4, mode->height / 4);
	glfwMaximizeWindow(im_window);
	glfwShowWindow(im_window);
	glfwGetWindowSize(im_window, &windowWidth, &windowHeight);

	if(!im_window){
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(im_window);

	glfwSetWindowSizeCallback(im_window, resize_callback);
	glfwSetScrollCallback(im_window, ScrollCallback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	glfwSwapInterval(0);
}

void App::Run(){
	IScene* scene = new Scene();

	static bool isTab = false;
	static bool isF1 = false;

	im_timer.startTimer();
	while(!endLoop){
		if(glfwWindowShouldClose(im_window) || Key(VK_ESCAPE)){
			endLoop = true;
		}

		scene->Update(im_timer.getElapsedTime());

		if(!isTab && Key(VK_TAB)){
			glfwGetWindowAttrib(im_window, GLFW_VISIBLE) ? glfwHideWindow(im_window) : glfwShowWindow(im_window);
			isTab = true;
		} else if(isTab && !Key(VK_TAB)){
			isTab = false;
		}
		if(!isF1 && Key(VK_F1)){
			const GLFWvidmode* const& mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwSetWindowMonitor(im_window, glfwGetWindowMonitor(im_window) ? nullptr : glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
			isF1 = true;
		} else if(isF1 && !Key(VK_F1)){
			isF1 = false;
		}

		if(glfwGetWindowAttrib(im_window, GLFW_VISIBLE)){
			glViewport(0, 0, windowWidth, windowHeight);
			scene->Render();
		}

		glfwSwapBuffers(im_window);
		glfwPollEvents();
        //im_timer.waitUntil(frameTime);
	}

	if(scene){
		delete scene;
		scene = nullptr;
	}
}

void App::Exit(){
	glfwDestroyWindow(im_window);
	glfwTerminate();
}