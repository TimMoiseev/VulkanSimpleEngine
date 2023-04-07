#pragma once
#include <GLFW/glfw3.h>

namespace vse {

	class VseWindow {
	public:
		VseWindow();
		~VseWindow();
		bool shouldClose();
		GLFWwindow* window;
	private:
		
		
		void initWindow();
	};
}