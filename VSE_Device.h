#pragma once
#include "VSE_Window.h"



namespace vse {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};

	class VseDevice {
	public:
		VseDevice( VseWindow& window);
		~VseDevice();
		void initVulkan();
		
		VseWindow& refWindow;
		VkInstance instance;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; // физическое устройство
		VkDevice device; // логическое устройство
		QueueFamilyIndices indices;
	private:
		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		VkQueue graphicsQueue; //дескриптор очереди с поддержкой граффических команд
		VkQueue presentQueue; //Дескриптор очереди с поддержкой отображения
		void createLogicalDevice();
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		void createInstance();
		bool checkValidationLayerSupport();
		std::vector<const char*> getRequiredExtensions();
		void pickPhysicalDevice();
		bool isDeviceSuitable(VkPhysicalDevice device);
	};
}