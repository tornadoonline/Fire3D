#include "application.h"

namespace Fire3D {

	void Application::run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanUp();
	}

	void Application::initWindow() {
		m_window = Window::create(WIDTH, HEIGHT);
	}

	void Application::initVulkan() {
		m_instance = Instance::create();
		m_surface = WindowSurface::create(m_instance, m_window);

		m_device = Device::create(m_instance, m_surface);

		m_swapchain = SwapChain::create(m_device, m_window, m_surface);

		m_renderPass = RenderPass::create(m_device);
		createRenderPass();

		m_swapchain->createFrameBuffers(m_renderPass);

		m_pipeline = Pipeline::create(m_device, m_renderPass);
		createPipeline();

		m_commandPool = CommandPool::create(m_device);

		m_commandBuffers.resize(m_swapchain->getImageCount());

		for (int i = 0; i < m_swapchain->getImageCount(); ++i) {
			m_commandBuffers[i] = CommandBuffer::create(m_device, m_commandPool);

			m_commandBuffers[i]->begin();

			VkRenderPassBeginInfo renderBeginInfo{};
			renderBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderBeginInfo.renderPass = m_renderPass->getRenderPass();
			renderBeginInfo.framebuffer = m_swapchain->getFrameBuffer(i);
			renderBeginInfo.renderArea.offset = {0, 0};
			renderBeginInfo.renderArea.extent = m_swapchain->getExtent();

			VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			renderBeginInfo.clearValueCount = 1;
			renderBeginInfo.pClearValues = &clearColor;


			m_commandBuffers[i]->beginRenderPass(renderBeginInfo);

			m_commandBuffers[i]->bindGraphicPipeline(m_pipeline->getPipeline());

			m_commandBuffers[i]->draw(3);

			m_commandBuffers[i]->endRenderPass();

			m_commandBuffers[i]->end();
		}

		for (int i = 0; i < m_swapchain->getImageCount(); ++i) {
			auto imageSemaphore = Semaphore::create(m_device);
			m_imageAvailableSemaphores.push_back(imageSemaphore);

			auto renderSemaphore = Semaphore::create(m_device);
			m_renderFinishedSemaphores.push_back(renderSemaphore);

			auto fence = Fence::create(m_device);
			m_fences.push_back(fence);
		}
	}

	void Application::createPipeline() {
		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)WIDTH;
		viewport.height = (float)HEIGHT;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor = {};
		scissor.offset = {0, 0};
		scissor.extent = {WIDTH, HEIGHT};

		m_pipeline->setViewports({ viewport });
		m_pipeline->setScissors({scissor});

		std::vector<Shader::Ptr> shaderGroup{};

		auto shaderVertex = Shader::create(m_device, "shaders/normal.vert.spv", VK_SHADER_STAGE_VERTEX_BIT, "main");
		shaderGroup.push_back(shaderVertex);

		auto shaderFragment = Shader::create(m_device, "shaders/normal.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT, "main");
		shaderGroup.push_back(shaderFragment);
		
		m_pipeline->setShaderGroup(shaderGroup);

		m_pipeline->mVertexInputState.vertexBindingDescriptionCount = 0;
		m_pipeline->mVertexInputState.pVertexBindingDescriptions = nullptr;
		m_pipeline->mVertexInputState.vertexAttributeDescriptionCount = 0;
		m_pipeline->mVertexInputState.pVertexAttributeDescriptions = nullptr;

		m_pipeline->mAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		m_pipeline->mAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		m_pipeline->mAssemblyState.primitiveRestartEnable = VK_FALSE;

		m_pipeline->mRasterState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		m_pipeline->mRasterState.polygonMode = VK_POLYGON_MODE_FILL;
		m_pipeline->mRasterState.lineWidth = 1.0f;
		m_pipeline->mRasterState.cullMode = VK_CULL_MODE_BACK_BIT;
		m_pipeline->mRasterState.frontFace = VK_FRONT_FACE_CLOCKWISE;

		m_pipeline->mRasterState.depthBiasEnable = VK_FALSE;
		m_pipeline->mRasterState.depthBiasConstantFactor = 0.0f;
		m_pipeline->mRasterState.depthBiasClamp = 0.0f;
		m_pipeline->mRasterState.depthBiasSlopeFactor = 0.0f;

		m_pipeline->mSampleState.sampleShadingEnable = VK_FALSE;
		m_pipeline->mSampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		m_pipeline->mSampleState.minSampleShading = 1.0f;
		m_pipeline->mSampleState.pSampleMask = nullptr;
		m_pipeline->mSampleState.alphaToCoverageEnable = VK_FALSE;
		m_pipeline->mSampleState.alphaToOneEnable = VK_FALSE;



		VkPipelineColorBlendAttachmentState blendAttachment{};
		blendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
			VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;

		blendAttachment.blendEnable = VK_FALSE;
		blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		blendAttachment.colorBlendOp = VK_BLEND_OP_ADD;

		blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		blendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		m_pipeline->pushBlendAttachment(blendAttachment);


		m_pipeline->mBlendState.logicOpEnable = VK_FALSE;
		m_pipeline->mBlendState.logicOp = VK_LOGIC_OP_COPY;

		m_pipeline->mBlendState.blendConstants[0] = 0.0f;
		m_pipeline->mBlendState.blendConstants[1] = 0.0f;
		m_pipeline->mBlendState.blendConstants[2] = 0.0f;
		m_pipeline->mBlendState.blendConstants[3] = 0.0f;

		m_pipeline->mLayoutState.setLayoutCount = 0;
		m_pipeline->mLayoutState.pSetLayouts = nullptr;
		m_pipeline->mLayoutState.pushConstantRangeCount = 0;
		m_pipeline->mLayoutState.pPushConstantRanges = nullptr;

		m_pipeline->build();
	}

	void Application::createRenderPass() {
		VkAttachmentDescription attachmentDes{};
		attachmentDes.format = m_swapchain->getFormat();
		attachmentDes.samples = VK_SAMPLE_COUNT_1_BIT;
		attachmentDes.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachmentDes.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachmentDes.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmentDes.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmentDes.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachmentDes.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		m_renderPass->addAttachment(attachmentDes);

		VkAttachmentReference attachmentRef{};
		attachmentRef.attachment = 0;
		attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		SubPass subPass{};
		subPass.addColorAttachmentReference(attachmentRef);
		subPass.buildSubPassDescription();

		m_renderPass->addSubPass(subPass);

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		m_renderPass->addDependency(dependency);

		m_renderPass->buildRenderPass();
	}


	void Application::mainLoop() {
		while (!m_window->shouldClose()) {
			m_window->pollEvents();

			render();
		}

		vkDeviceWaitIdle(m_device->getDevice());
	}

	void Application::render() {
		m_fences[m_currentFrame]->block();

		uint32_t imageIndex{ 0 };
		vkAcquireNextImageKHR(
			m_device->getDevice(),
			m_swapchain->getSwapChain(),
			UINT64_MAX,
			m_imageAvailableSemaphores[m_currentFrame]->getSemaphore(),
			VK_NULL_HANDLE,
			&imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		

		VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphores[m_currentFrame]->getSemaphore() };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphores[m_currentFrame]->getSemaphore()};

		auto commandBuffer = m_commandBuffers[imageIndex]->getCommandBuffer();


		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		m_fences[m_currentFrame]->resetFence();
		if (vkQueueSubmit(m_device->getGraphicQueue(), 1, &submitInfo, m_fences[m_currentFrame]->getFence()) != VK_SUCCESS) {
			throw std::runtime_error("Error:failed to submit renderCommand");
		}

		VkSwapchainKHR swapChains[] = {m_swapchain->getSwapChain()};

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		vkQueuePresentKHR(m_device->getPresentQueue(), &presentInfo);
		m_currentFrame = (m_currentFrame + 1) % m_swapchain->getImageCount();
	}

	void Application::cleanUp() {

		m_pipeline.reset();

		m_renderPass.reset();

		m_swapchain.reset();

		m_device.reset();
		m_surface.reset();
		m_instance.reset();
		m_window.reset();
	}
}