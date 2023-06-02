#pragma once

#include "base.h"
#include "instance.h"
#include "windowSurface.h"

namespace Fire3D {

	class Device {
	public:
		using Ptr = std::shared_ptr<Device>;
		static Ptr create(Instance::Ptr instance, WindowSurface::Ptr surface) { 
			return std::make_shared<Device>(instance, surface); 
		}

		Device(Instance::Ptr instance, WindowSurface::Ptr surface);

		~Device();

		void pickPhysicalDevice();

		int rateDevice(VkPhysicalDevice device);

		bool isDeviceSuitable(VkPhysicalDevice device);

		void initQueueFamilies(VkPhysicalDevice device);

		void createLogicalDevice();

		bool isQueueFamilyComplete();

		auto getDevice() const { return m_device; }
		auto getPhysicalDevice() const { return m_physicalDevice; }

		auto getGraphicQueueFamily() const { return m_graphicQueueFamily; }
		auto getPresentQueueFamily() const { return m_presentQueueFamily; }

		auto getGraphicQueue() const { return m_graphicQueue; }
		auto getPresentQueue() const { return m_presentQueue; }
	private:
		VkPhysicalDevice m_physicalDevice{ VK_NULL_HANDLE };
		Instance::Ptr m_instance{ nullptr };
		WindowSurface::Ptr m_surface{ nullptr };

		std::optional<uint32_t> m_graphicQueueFamily;
		VkQueue	m_graphicQueue{ VK_NULL_HANDLE };

		std::optional<uint32_t> m_presentQueueFamily;
		VkQueue m_presentQueue{ VK_NULL_HANDLE };

		VkDevice m_device{ VK_NULL_HANDLE };
	};
}