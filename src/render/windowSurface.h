#pragma once

#include "base.h"
#include "instance.h"
#include "window.h"

namespace Fire3D {

	class WindowSurface {
	public:
		using Ptr = std::shared_ptr<WindowSurface>;
		static Ptr create(Instance::Ptr instance, Window::Ptr window) {
			return std::make_shared<WindowSurface>(instance, window);
		}

		WindowSurface(Instance::Ptr instance, Window::Ptr window);

		~WindowSurface();

		auto getSurface() const { return m_surface; }

	private:
		VkSurfaceKHR m_surface{ VK_NULL_HANDLE };
		Instance::Ptr m_instance{ nullptr };
	};
}
