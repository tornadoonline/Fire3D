#pragma once

#include "base.h"
#include "instance.h"
#include "device.h"
#include "window.h"
#include "windowSurface.h"
#include "swapChain.h"
#include "shader.h"
#include "pipeline.h"
#include "renderPass.h"
#include "commandPool.h"
#include "commandBuffer.h"
#include "semaphore.h"
#include "fence.h"

namespace Fire3D {

	const int WIDTH = 800;
	const int HEIGHT = 600;

	class Application {
	public:
		Application() = default;

		~Application() = default;

		void run();

	private:
		void initWindow();

		void initVulkan();

		void mainLoop();

		void render();

		void cleanUp();

	private:
		void createPipeline();
		void createRenderPass();

	private:
		int m_currentFrame{ 0 };
		Window::Ptr m_window{ nullptr };
		Instance::Ptr m_instance{ nullptr };
		Device::Ptr m_device{ nullptr };
		WindowSurface::Ptr m_surface{ nullptr };
		SwapChain::Ptr m_swapchain{ nullptr };
		Pipeline::Ptr m_pipeline{ nullptr };
		RenderPass::Ptr m_renderPass{ nullptr };
		CommandPool::Ptr m_commandPool{ nullptr };

		std::vector<CommandBuffer::Ptr> m_commandBuffers{};

		std::vector<Semaphore::Ptr> m_imageAvailableSemaphores{};
		std::vector<Semaphore::Ptr> m_renderFinishedSemaphores{};
		std::vector<Fence::Ptr> m_fences{};
	};
}