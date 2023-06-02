#include "fence.h"

namespace Fire3D {

	Fence::Fence(const Device::Ptr& device, bool signaled) {
		m_device = device;

		VkFenceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		createInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

		if (vkCreateFence(m_device->getDevice(), &createInfo, nullptr, &mFence) != VK_SUCCESS) {
			throw std::runtime_error("Error:failed to create fence");
		}
	}

	Fence::~Fence() {
		if (mFence != VK_NULL_HANDLE) {
			vkDestroyFence(m_device->getDevice(), mFence, nullptr);
		}
	}

	void Fence::resetFence() {
		vkResetFences(m_device->getDevice(), 1, &mFence);
	}

	void Fence::block(uint64_t timeout) {
		vkWaitForFences(m_device->getDevice(), 1, &mFence, VK_TRUE, timeout);
	}
}