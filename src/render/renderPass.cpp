#include "renderPass.h"

namespace Fire3D {

	SubPass::SubPass() {

	}

	SubPass::~SubPass() {

	}

	void SubPass::addColorAttachmentReference(const VkAttachmentReference& ref) {
		mColorAttachmentReferences.push_back(ref);
	}

	void SubPass::addInputAttachmentReference(const VkAttachmentReference& ref) {
		mInputAttachmentReferences.push_back(ref);
	}

	void SubPass::setDepthStencilAttachmentReference(const VkAttachmentReference& ref) {
		mDepthStencilAttachmentReference = ref;
	}

	void SubPass::buildSubPassDescription() {
		if (mColorAttachmentReferences.empty()) {
			throw std::runtime_error("Error: color attachment group is empty!");
		}
		mSubPassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		mSubPassDescription.colorAttachmentCount = static_cast<uint32_t>(mColorAttachmentReferences.size());
		mSubPassDescription.pColorAttachments = mColorAttachmentReferences.data();

		mSubPassDescription.inputAttachmentCount = static_cast<uint32_t>(mInputAttachmentReferences.size());
		mSubPassDescription.pInputAttachments = mInputAttachmentReferences.data();

		mSubPassDescription.pDepthStencilAttachment = mDepthStencilAttachmentReference.layout == VK_IMAGE_LAYOUT_UNDEFINED? nullptr : &mDepthStencilAttachmentReference;
	}


	RenderPass::RenderPass(const Device::Ptr& device) {
		m_device = device;
	}

	RenderPass::~RenderPass() {
		if (m_renderPass != VK_NULL_HANDLE) {
			vkDestroyRenderPass(m_device->getDevice(), m_renderPass, nullptr);
		}
	}

	void RenderPass::addSubPass(const SubPass& subpass) { mSubPasses.push_back(subpass); }

	void RenderPass::addDependency(const VkSubpassDependency& dependency) { mDependencies.push_back(dependency); }

	void RenderPass::addAttachment(const VkAttachmentDescription& attachmentDes) { mAttachmentDescriptions.push_back(attachmentDes); }

	void RenderPass::buildRenderPass() {
		if (mSubPasses.empty() || mAttachmentDescriptions.empty() || mDependencies.empty()) {
			throw std::runtime_error("Error: not enough elements to build renderPass");
		}

		//unwrap
		std::vector<VkSubpassDescription> subPasses{};
		for (int i = 0; i < mSubPasses.size(); ++i) {
			subPasses.push_back(mSubPasses[i].getSubPassDescription());
		}

		VkRenderPassCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

		createInfo.attachmentCount = static_cast<uint32_t>(mAttachmentDescriptions.size());
		createInfo.pAttachments = mAttachmentDescriptions.data();

		createInfo.dependencyCount = static_cast<uint32_t>(mDependencies.size());
		createInfo.pDependencies = mDependencies.data();

		createInfo.subpassCount = static_cast<uint32_t>(subPasses.size());
		createInfo.pSubpasses = subPasses.data();

		if (vkCreateRenderPass(m_device->getDevice(), &createInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed to create renderPass");
		}
	}
}