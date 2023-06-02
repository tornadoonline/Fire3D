#include "semaphore.h"

namespace Fire3D {

	Semaphore::Semaphore(const Device::Ptr& device) {
		m_device = device;

		VkSemaphoreCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		if (vkCreateSemaphore(m_device->getDevice(), &createInfo, nullptr, &mSemaphore) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed to create Semaphore");
		}
	}

	Semaphore::~Semaphore() {
		if (mSemaphore != VK_NULL_HANDLE) {
			vkDestroySemaphore(m_device->getDevice(), mSemaphore, nullptr);
		}
	}
}