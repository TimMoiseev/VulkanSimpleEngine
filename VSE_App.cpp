#include "VSE_App.h"

void vse::VseApp::run()
{
	while (!vseWindow.shouldClose()) {
		glfwPollEvents();
	};
}

vse::VseApp::VseApp()
{
    std::cout << "vseApp constructor call" << std::endl;
}

vse::VseApp::~VseApp()
{    
    std::cout << "vseApp destructor call" << std::endl;
}
