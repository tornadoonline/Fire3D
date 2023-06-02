#include "commandPool.h"

namespace Fire3D {

	CommandPool::CommandPool(const Device::Ptr& device, VkCommandPoolCreateFlagBits flag) {
		m_device = device;

		VkCommandPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.queueFamilyIndex = device->getGraphicQueueFamily().value();

		createInfo.flags = flag;

		if (vkCreateCommandPool(m_device->getDevice(), &createInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
			throw std::runtime_error("Error:  failed to create command pool");
		}
	}

	CommandPool::~CommandPool() {
		if (m_commandPool != VK_NULL_HANDLE) {
			vkDestroyCommandPool(m_device->getDevice(), m_commandPool, nullptr);
		}
	}
}