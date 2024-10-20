#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getPata1() { return pata1; };
	GLfloat getPata2() { return pata2; };
	GLfloat getPata3() { return pata3; };
	GLfloat getStateDado() { return stateDado; };
	GLfloat getLuz() { return luz; };
	GLfloat getCarLight() { return luzcarro; };
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }


	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLfloat pata1 = 0.0f;
	GLfloat pata2 = 0.0f;
	GLfloat pata3 = 0.0f;
	GLint stateDado = 4;
	GLfloat luz = 0.0f;
	GLfloat luzcarro = 0.0f;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};
