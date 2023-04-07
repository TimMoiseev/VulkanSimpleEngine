#pragma once
#include "VSE_Window.h"
#include <vector>
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <iostream>
#include <stdexcept>
#include <cstdlib>

//#include "VSE_DebugMessenger.h"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};




namespace vse {
	class VseDevice {
	public:
		VseDevice();
		~VseDevice();
		void initVulkan();
		VkInstance instance;
		
	private:
		/*VseWindow vseWindow;*/
		void createInstance();
		bool checkValidationLayerSupport();
		std::vector<const char*> getRequiredExtensions();
		
		
	};
}