#include "VSE_Window.h"
#include <iostream>

namespace vse {
	VseWindow::VseWindow()
	{
		std::cout << "vseWindow constructor call" << std::endl;
		initWindow();
	}
	VseWindow::~VseWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		std::cout << "vseWindow destructor call" << std::endl;

	}
	bool VseWindow::shouldClose()
	{
		return glfwWindowShouldClose(window);;
	}
	void VseWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //не используем opengl api
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(800, 600, "VSE", nullptr, nullptr);
	}
}

