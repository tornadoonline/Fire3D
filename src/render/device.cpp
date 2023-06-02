#include "device.h"
#include "renderglobal.hpp"


namespace Fire3D {

	Device::Device(Instance::Ptr instance, WindowSurface::Ptr surface) {
		m_instance = instance;
		m_surface = surface;
		pickPhysicalDevice();
		initQueueFamilies(m_physicalDevice);
		createLogicalDevice();
	}

	Device::~Device() {
		vkDestroyDevice(m_device, nullptr);
		m_surface.reset();
		m_instance.reset();
	}

	void Device::pickPhysicalDevice() {
		uint32_t physicalDeviceCount = 0;
		vkEnumeratePhysicalDevices(m_instance->getInstance(), &physicalDeviceCount, nullptr);

		if (physicalDeviceCount == 0) {
			throw std::runtime_error("Error:failed to enumeratePhysicalDevice");
		}

		std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		vkEnumeratePhysicalDevices(m_instance->getInstance(), &physicalDeviceCount, physicalDevices.data());

		std::multimap<int, VkPhysicalDevice> candidates;
		for (const auto& physicalDevice : physicalDevices) {
			int score = rateDevice(physicalDevice);
			candidates.insert(std::make_pair(score, physicalDevice));
		}

		auto [score, physicalDevice] = *(std::rbegin(candidates));

		if (score > 0 && isDeviceSuitable(physicalDevice)) {
			m_physicalDevice = physicalDevice;
		}

		if (m_physicalDevice == VK_NULL_HANDLE) {
			throw std::runtime_error("Error:failed to get physical device");
		}
	}

	int Device::rateDevice(VkPhysicalDevice physicalDevice) {
		int score = 0;

		VkPhysicalDeviceProperties  physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

		VkPhysicalDeviceFeatures physicalDeviceFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &physicalDeviceFeatures);

		if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			score += 1000;
		}

		score += physicalDeviceProperties.limits.maxImageDimension2D;

		if (!physicalDeviceFeatures.geometryShader) {
			return 0;
		}

		return score;
	}

	bool Device::isDeviceSuitable(VkPhysicalDevice physicalDevice) {
		VkPhysicalDeviceProperties  physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

		VkPhysicalDeviceFeatures physicalDeviceFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &physicalDeviceFeatures);

		return physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
			physicalDeviceFeatures.geometryShader;
	}

	void Device::initQueueFamilies(VkPhysicalDevice physicalDevice) {
		uint32_t queueFamilyPropertyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamiliesProperties(queueFamilyPropertyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, queueFamiliesProperties.data());

		int i = 0;
		for (const auto& queueFamilyProperties : queueFamiliesProperties) {
			if (queueFamilyProperties.queueCount > 0 && (queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
				m_graphicQueueFamily = i;
			}

			VkBool32 presentSupport = VK_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, m_surface->getSurface(), &presentSupport);

			if (presentSupport) {
				m_presentQueueFamily = i;
			}

			if (isQueueFamilyComplete()) {
				break;
			}

			++i;
		}
	}

	void Device::createLogicalDevice() {
		std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos{};

		std::set<uint32_t> queueFamilies = {m_graphicQueueFamily.value(), m_presentQueueFamily.value()};

		float queuePriority = 1.0;

		for (uint32_t queueFamily : queueFamilies) {
			VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
			deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			deviceQueueCreateInfo.queueFamilyIndex = queueFamily;
			deviceQueueCreateInfo.queueCount = 1;
			deviceQueueCreateInfo.pQueuePriorities = &queuePriority;

			deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
		}
		

		VkPhysicalDeviceFeatures physicalDeviceFeatures = {};

		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(deviceQueueCreateInfos.size());
		deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(g_requiredDeviceExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = g_requiredDeviceExtensions.data();

		if (g_enableValidationLayer) {
			deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(g_validationLayerSupport.size());
			deviceCreateInfo.ppEnabledLayerNames = g_validationLayerSupport.data();
		}
		else {
			deviceCreateInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device) != VK_SUCCESS) {
			throw std::runtime_error("Error:failed to create logical device");
		}

		vkGetDeviceQueue(m_device, m_graphicQueueFamily.value(), 0, &m_graphicQueue);
		vkGetDeviceQueue(m_device, m_presentQueueFamily.value(), 0, &m_presentQueue);
	}

	bool Device::isQueueFamilyComplete() {
		return m_graphicQueueFamily.has_value() && m_presentQueueFamily.has_value();
	}
}