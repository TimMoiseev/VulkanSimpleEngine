#include "VSE_Window.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
namespace vse {
	VseWindow::VseWindow()
	{
		std::cout << "vseWindow constructor call" << std::endl;
		createWindow();
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
	void VseWindow::createWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //не используем opengl api
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(800, 600, "VSE", nullptr, nullptr);
		
	}
	void VseWindow::createWindowSurface(VkInstance* instance)
	{
		VkWin32SurfaceCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hwnd = glfwGetWin32Window(window);
		createInfo.hinstance = GetModuleHandle(nullptr);
		if (vkCreateWin32SurfaceKHR(*instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}
}

