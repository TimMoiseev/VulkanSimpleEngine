#pragma once
#include <vector>
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <optional>
#include <GLFW/glfw3.h>


namespace vse {

	class VseWindow {
	public:
		VseWindow();
		~VseWindow();
		bool shouldClose();
		GLFWwindow* window;
		void createWindowSurface(VkInstance* instancce);
		VkSurfaceKHR surface;
	private:
		void createWindow();
		
		

	};
}