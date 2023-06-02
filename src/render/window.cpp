#include "window.h"

namespace Fire3D {

	Window::Window(const int& width, const int& height) {
		m_width = width;
		m_height = height;

		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_window = glfwCreateWindow(m_width, m_height, "vulkan window", nullptr, nullptr);
		if (!m_window) {
			std::cerr << "Error: failed to create window" << std::endl;
		}
	}

	Window::~Window() {
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	bool Window::shouldClose() {
		return glfwWindowShouldClose(m_window);
	}

	void Window::pollEvents() {
		glfwPollEvents();
	}
}