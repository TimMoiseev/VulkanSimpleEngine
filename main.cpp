#include "VSE_App.h"
using namespace vse;


//class HelloTriangleApp {
//public:
//	void run() {
//		initWindow();
//		initVulkan();
//		mainLoop();
//		cleanUp();
//	}
//private:
//	GLFWwindow* window;
//	VkInstance instance;
//
//	void initWindow() {
//		glfwInit();
//		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //не используем opengl api
//		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
//		window = glfwCreateWindow(800, 600, "VSE", nullptr, nullptr);
//	}
//	void initVulkan() {
//		createInstance();
//	}
//
//	void createInstance() {
//		//создаем инстанс вулкана, заполняем структуру с настройками инстанса
//		VkApplicationInfo appInfo{}; // Value initialization что бы заполнить первоначально все нулями
//		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//		appInfo.pApplicationName = "Hello Vulkan";
//		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
//		appInfo.pEngineName = "Vulkan Simple Engine";
//		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
//		appInfo.apiVersion = VK_API_VERSION_1_3;
//
//		VkInstanceCreateInfo createInfo{};
//		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//		createInfo.pApplicationInfo = &appInfo;
//
//		uint32_t glfwExtensionCount = 0;
//		const char** glfwExtensions;
//
//		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); // возвращает список и колличество расширениий требуемых для glfw
//		createInfo.enabledExtensionCount = glfwExtensionCount;
//		createInfo.ppEnabledExtensionNames = glfwExtensions;
//		createInfo.enabledLayerCount = 0; //глобальные слои валидации пока не включаем
//
//		uint32_t extensionCount = 0;
//		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
//		std::vector<VkExtensionProperties> extensions(extensionCount);
//		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
//		
//		
//		std::cout << "available extensions:\n";
//
//		for (const auto& extension : extensions) {
//			std::cout << '\t' << extension.extensionName << '\n';
//		}
//		
//
//		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) { //создаем инстанс, передаем в метод по ссылке структуру createInfo, записываем полученный инстанс по сылке в приватное поле
//			throw std::runtime_error("failed to create instance!");
//		}
//
//	}
//
//	void mainLoop() {
//		while (!glfwWindowShouldClose(window)) {
//			glfwPollEvents();
//		}
//	}
//
//	void cleanUp() {
//		vkDestroyInstance(instance, nullptr);
//		glfwDestroyWindow(window);
//		glfwTerminate();
//	}
//};

int main() {
	VseApp app;
	try {
		app.run();
		
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}