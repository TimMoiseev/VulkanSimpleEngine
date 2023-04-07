#include "VSE_Device.h"
#include "VSE_DebugMessenger.h"

vse::VseDevice::VseDevice()
{
	std::cout << "vseDevice constructor call" << std::endl;
	initVulkan();
}

vse::VseDevice::~VseDevice()
{
	
	if (enableValidationLayers) {
		vse::VseDebugMessenger::createInstance()->DestroyDebugUtilsMessengerEXT(instance, vse::VseDebugMessenger::createInstance()->debugMessenger, nullptr);
		std::cout << "VseDebugMessenger destroyed" << std::endl;
	}
	vkDestroyInstance(instance, nullptr);
	std::cout << "vseDevice destructor call" << std::endl;
	
}

void vse::VseDevice::initVulkan()
{
	
	createInstance();
	vse::VseDebugMessenger::createInstance()->setupDebugMessenger(instance);
	
}

void vse::VseDevice::createInstance()
{
	if (enableValidationLayers && !checkValidationLayerSupport()) {
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
		if (enableValidationLayers) {
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

bool vse::VseDevice::checkValidationLayerSupport() {
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

std::vector<const char*> vse::VseDevice::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); // возвращает список и колличество расширениий требуемых для glfw
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		//если слои валидации включены (включены в дебаг режиме)
		//добавляем макрос 
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // макрос, расширяется в полное имя расширения ответственного дебаг утилиты(колбэк отладочных сообщений и т.д) "VK_EXT_debug_utils"
	}
	return extensions;
}
