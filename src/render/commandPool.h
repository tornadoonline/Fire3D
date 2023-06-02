#pragma once

#include "base.h"
#include "device.h"

namespace Fire3D {

	class CommandPool {
	public:
		using Ptr = std::shared_ptr<CommandPool>;
		static Ptr create(const Device::Ptr& device, VkCommandPoolCreateFlagBits flag = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) {
			return std::make_shared<CommandPool>(device, flag); 
		}

		CommandPool(const Device::Ptr &device, VkCommandPoolCreateFlagBits flag = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

		~CommandPool();

		auto getCommandPool() const { return m_commandPool; }

	private:
		VkCommandPool m_commandPool{ VK_NULL_HANDLE };
		Device::Ptr m_device{ nullptr };
	};
}