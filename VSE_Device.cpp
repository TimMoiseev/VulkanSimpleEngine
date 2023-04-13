#include "VSE_Device.h"
#include "VSE_DebugMessenger.h"
#include <GLFW/glfw3.h>
#include <set>
#define GLFW_INCLUDE_VULKAN
namespace vse {

	VseDevice::VseDevice(VseWindow& window) : refWindow{ window }
	{
		std::cout << "vseDevice constructor call" << std::endl;
		initVulkan();
	}

	VseDevice::~VseDevice()
	{
		vkDestroySurfaceKHR(instance, refWindow.surface, nullptr);
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
		refWindow.createWindowSurface(&instance);
		pickPhysicalDevice();
		createLogicalDevice();
		
	}

	void VseDevice::createLogicalDevice()
	{
		 indices = findQueueFamilies(physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}
		
		VkPhysicalDeviceFeatures deviceFeatures{};
		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
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
		vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue); //�������� ���������� ������� ����������� ������
		vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue); // �������� ���������� ������� �������������� �����������
	}

	QueueFamilyIndices VseDevice::findQueueFamilies(VkPhysicalDevice device)
	{
		//����� ���������� ������� ��������� �������� �� ����������� ����������
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
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, refWindow.surface, &presentSupport);
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
		//������� ������� �������, ��������� ��������� � ����������� ��������
		VkApplicationInfo appInfo{}; // Value initialization ��� �� ��������� ������������� ��� ������
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
			//������� �������, �������� � ����� �� ������ ��������� createInfo, ���������� ���������� ������� �� ����� � ��������� ����
			throw std::runtime_error("failed to create instance!");
		}
	}

	bool VseDevice::checkValidationLayerSupport() {
		//��������, ������������ �� ���� ������� �� ����� � ������ �������������� ��������
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
			if (!layerFound) { return false; } //���� ���� ���� �� ����� ������ ����
		}
		return true;
	}

	std::vector<const char*> VseDevice::getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); // ���������� ������ � ����������� ����������� ��������� ��� glfw
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		
		if (debugMode) {
			//���� ���� ��������� �������� (�������� � ����� ������)
			//��������� ������ 
			
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // ������, ����������� � ������ ��� ���������� �������������� �� ����� �������(������ ���������� ��������� � �.�) "VK_EXT_debug_utils"
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

	bool VseDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) {
		
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}
		//���� ��� ���������� ������ �� �������� �������(requiredExtensions) ��� ���� � availableExtensions �� ������� �� �� ����
		return requiredExtensions.empty();
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
		bool extensionsSupported = checkDeviceExtensionSupport(device);
		//return true ���� ����� ��������� ������� ������(graphics and present) � ��������� ���������� �������������� ���������� ������������
		return indices.isComplete() && extensionsSupported;
	}
}