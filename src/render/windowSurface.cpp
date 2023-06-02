#include"windowSurface.h"

namespace Fire3D {

	WindowSurface::WindowSurface(Instance::Ptr instance, Window::Ptr window) {
		m_instance = instance;
		if (glfwCreateWindowSurface(instance->getInstance(), window->getWindow(), nullptr, &m_surface) != VK_SUCCESS) {
			throw std::runtime_error("Error: failed to create surface");
		}
	}

	WindowSurface::~WindowSurface() {
		vkDestroySurfaceKHR(m_instance->getInstance(), m_surface, nullptr);
		m_instance.reset();
	}

}