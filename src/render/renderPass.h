#pragma once

#include "base.h"
#include "device.h"

namespace Fire3D {


	class SubPass {
	public:
		SubPass();

		~SubPass();

		void addColorAttachmentReference(const VkAttachmentReference& ref);

		void addInputAttachmentReference(const VkAttachmentReference& ref);

		void setDepthStencilAttachmentReference(const VkAttachmentReference& ref);

		void buildSubPassDescription();

		auto getSubPassDescription() const { return mSubPassDescription; }

	private:
		VkSubpassDescription mSubPassDescription{};
		std::vector<VkAttachmentReference> mColorAttachmentReferences{};
		std::vector<VkAttachmentReference> mInputAttachmentReferences{};
		VkAttachmentReference mDepthStencilAttachmentReference{};
	};

	class RenderPass {
	public:
		using Ptr = std::shared_ptr<RenderPass>;

		static Ptr create(const Device::Ptr& device) { return std::make_shared<RenderPass>(device); }
		
		RenderPass(const Device::Ptr &device);

		~RenderPass();

		void addSubPass(const SubPass& subpass);

		void addDependency(const VkSubpassDependency &dependency);

		void addAttachment(const VkAttachmentDescription &attachmentDes);

		void buildRenderPass();

		auto getRenderPass() const { return m_renderPass; }

	private:
		VkRenderPass m_renderPass{VK_NULL_HANDLE};

		std::vector<SubPass> mSubPasses{};
		std::vector<VkSubpassDependency> mDependencies{};
		std::vector<VkAttachmentDescription> mAttachmentDescriptions{};

		Device::Ptr m_device{ nullptr };
	};
}