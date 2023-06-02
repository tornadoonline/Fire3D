#include "instance.h"
#include "renderglobal.hpp"
#include "renderutils.h"

namespace Fire3D {
	

	Instance::Instance() {

		if (g_enableValidationLayer && !checkValidationLayerSupport()) {
			throw std::runtime_error("Error: validation layer is not supported");
		}

		printAvailableExtensions();

		VkApplicationInfo applicationInfo = {};
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.pApplicationName = "Fire3D";
		applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.pEngineName = "NO ENGINE";
		applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &applicationInfo;

		auto extensions = getRequiredExtensions();
		instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

		if (g_enableValidationLayer) {
			instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(g_validationLayerSupport.size());
			instanceCreateInfo.ppEnabledLayerNames = g_validationLayerSupport.data();
		}
		else {
			instanceCreateInfo.enabledLayerCount = 0;
		}

		if (vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance) != VK_SUCCESS) {
			throw std::runtime_error("Error:failed to create instance");
		}

		setupDebugger();
	}

	Instance::~Instance() {
		if (g_enableValidationLayer) {
			DestroyDebugUtilsMessengerEXT(m_instance, m_debugUtilsMessengerEXT, nullptr);
		}
		
		vkDestroyInstance(m_instance, nullptr);
	}

	void Instance::printAvailableExtensions() {
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		std::cout << "Available extensions:" << std::endl;

		for (const auto& extension : extensions) {
			std::cout << extension.extensionName << std::endl;
		}
	}

	std::vector<const char*> Instance::getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;

		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		return extensions;
	}

	bool Instance::checkValidationLayerSupport() {
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const auto& layerName : g_validationLayerSupport) {
			bool layerFound = false;

			for (const auto& layerProp : availableLayers) {
				if (std::strcmp(layerName, layerProp.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	void Instance::setupDebugger() {
		if (!g_enableValidationLayer) { return; }

		VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT = {};
		debugUtilsMessengerCreateInfoEXT.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugUtilsMessengerCreateInfoEXT.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		debugUtilsMessengerCreateInfoEXT.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		debugUtilsMessengerCreateInfoEXT.pfnUserCallback = DebugUtilsMessengerCallbackEXT;
		debugUtilsMessengerCreateInfoEXT.pUserData = nullptr;

		if (CreateDebugUtilsMessengerEXT(m_instance, &debugUtilsMessengerCreateInfoEXT, nullptr, &m_debugUtilsMessengerEXT) != VK_SUCCESS) {
			throw std::runtime_error("Error:failed to create debugger");
		}
	}
}