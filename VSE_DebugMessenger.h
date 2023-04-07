#include <vulkan/vulkan.h>
#include <iostream>

#ifdef NDEBUG
const bool debugMode = false;
#else
const bool debugMode = true;
#endif

namespace vse {
	class VseDebugMessenger {
	public:
		static VseDebugMessenger* createInstance() {
			//синглетон Майерса
			static VseDebugMessenger instance;
			return &instance;
		}

		void setupDebugMessenger(VkInstance& instance);
		
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
		VkDebugUtilsMessengerEXT debugMessenger;
	private:
		VseDebugMessenger() { std::cout << "VseDebugMessenger created" << std::endl; };
		~VseDebugMessenger() {};
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	};
}