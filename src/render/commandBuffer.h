#pragma once

#include "base.h"
#include "commandPool.h"
#include "device.h"

namespace Fire3D {

	class CommandBuffer {
	public:
		using Ptr = std::shared_ptr<CommandBuffer>;
		static Ptr create(const Device::Ptr& device, const CommandPool::Ptr& commandPool, bool asSecondary = false) { 
			return std::make_shared<CommandBuffer>(device, commandPool, asSecondary); 
		}

		CommandBuffer(const Device::Ptr &device, const CommandPool::Ptr &commandPool, bool asSecondary = false);

		~CommandBuffer();


		
		void begin(VkCommandBufferUsageFlags flag = 0, const VkCommandBufferInheritanceInfo& inheritance = {});

		void beginRenderPass(const VkRenderPassBeginInfo &renderPassBeginInfo, const VkSubpassContents &subPassContents = VK_SUBPASS_CONTENTS_INLINE);

		void bindGraphicPipeline(const VkPipeline &pipeline);

		void draw(size_t vertexCount);

		void endRenderPass();

		void end();

		auto getCommandBuffer() const { return mCommandBuffer; }
	private:
		VkCommandBuffer mCommandBuffer{ VK_NULL_HANDLE };
		Device::Ptr m_device{ nullptr };
		CommandPool::Ptr m_commandPool{ nullptr };
	};
}