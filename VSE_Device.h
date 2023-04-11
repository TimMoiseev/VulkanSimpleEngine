#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <optional>
#include "VSE_Window.h"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

namespace vse {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	class VseDevice {
	public:
		VseDevice( VseWindow& window);
		~VseDevice();
		void initVulkan();
		void createLogicalDevice();
		VseWindow& vseWindow;
		VkInstance instance;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; // физическое устройство
		VkDevice device; // логическое устройство
		VkQueue graphicsQueue; //дескриптор очереди граффических команд
	private:
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		void createInstance();
		bool checkValidationLayerSupport();
		std::vector<const char*> getRequiredExtensions();
		void pickPhysicalDevice();
		bool isDeviceSuitable(VkPhysicalDevice device);
	};
}