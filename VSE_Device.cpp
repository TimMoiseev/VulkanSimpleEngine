#include "VSE_Device.h"
#include "VSE_DebugMessenger.h"
#include <GLFW/glfw3.h>
#define GLFW_INCLUDE_VULKAN
namespace vse {

	VseDevice::VseDevice(VseWindow& window) : vseWindow{ window }
	{
		std::cout << "vseDevice constructor call" << std::endl;
		initVulkan();
	}

	VseDevice::~VseDevice()
	{
		vkDestroySurfaceKHR(instance, vseWindow.surface, nullptr);
		vkDestroyDevice(device, nullptr);
		if (debugMode) {
			VseDebugMessenger::createInstance()->DestroyDebugUtilsMessengerEXT(instance, VseDebugMessenger::createInstance()->debugMessenger, nullptr);
			std::cout << "VseDebugMessenger destroyed" << std::endl;
		}
		vkDestroyInstance(instance, nullptr);
		std::cout << "vseDevice destructor call" << std::endl;
	}

	void VseDevice::initVulkan()
	{
		createInstance();
		VseDebugMessenger::createInstance()->setupDebugMessenger(instance);
		vseWindow.createWindowSurface(&instance);
		pickPhysicalDevice();
		createLogicalDevice();
		
	}

	void VseDevice::createLogicalDevice()
	{
		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
		queueCreateInfo.queueCount = 1;
		float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		VkPhysicalDeviceFeatures deviceFeatures{};
		VkDeviceCreateInfo createInfo{};
		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = 0;
		if (debugMode) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}
		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
			throw std::runtime_error("failed to create logical device");
		}
		vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue); //ролучаем дескриптор очереди графических команд
	}

	QueueFamilyIndices VseDevice::findQueueFamilies(VkPhysicalDevice device)
	{
		//метод возвращает индексы семейства очередей по физическому устройству
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& family : queueFamilies) {
			if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vseWindow.surface, &presentSupport);
			if (presentSupport) {
				indices.presentFamily = i;
			}
			if (indices.isComplete()) {
				break;
			}
			i++;
		}
		return indices;
	}

	void VseDevice::createInstance()
	{
		if (debugMode && !checkValidationLayerSupport()) {
			throw std::runtime_error("validation layers requested, but not available!");
		}
		//создаем инстанс вулкана, заполняем структуру с настройками инстанса
		VkApplicationInfo appInfo{}; // Value initialization что бы заполнить первоначально все нулями
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Vulkan";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Vulkan Simple Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (debugMode) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();

			vse::VseDebugMessenger::createInstance()->populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			//создаем инстанс, передаем в метод по ссылке структуру createInfo, записываем полученный инстанс по сылке в приватное поле
			throw std::runtime_error("failed to create instance!");
		}
	}

	bool VseDevice::checkValidationLayerSupport() {
		//проверка, присутствуют ли слои которые мы хотим в списке поддерживаемых вулканом
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;
			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}
			if (!layerFound) { return false; } //если хоть одно не нашли вернем ложь
		}
		return true;
	}

	std::vector<const char*> VseDevice::getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); // возвращает список и колличество расширениий требуемых для glfw
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		auto result = glfwVulkanSupported();
		if (debugMode) {
			//если слои валидации включены (включены в дебаг режиме)
			//добавляем макрос 
			
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // макрос, расширяется в полное имя расширения ответственного за дебаг утилиты(колбэк отладочных сообщений и т.д) "VK_EXT_debug_utils"
		}
		return extensions;
	}

	void VseDevice::pickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		if (deviceCount == 0) {
			throw std::runtime_error("failed to find GPUs with Vulkan support");
		}
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
		for (const auto& device : devices) {
			if (isDeviceSuitable(device)) {
				physicalDevice = device;
				break;
			}
			if (physicalDevice == VK_NULL_HANDLE) {
				throw std::runtime_error("failed to find a suitable GPU");
			}
		}

	}

	bool VseDevice::isDeviceSuitable(VkPhysicalDevice device)
	{
		if (debugMode) {
			VkPhysicalDeviceProperties deviceProperties;
			VkPhysicalDeviceFeatures deviceFeatures;

			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
			std::cout << "device name: " << deviceProperties.deviceName << std::endl;
			std::cout << "device api version: " << deviceProperties.apiVersion << std::endl;
		}

		QueueFamilyIndices indices = findQueueFamilies(device);
		return indices.isComplete();
	}
}