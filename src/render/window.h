#pragma once

#include "base.h"

namespace Fire3D {

	class Window {
	public:
		using Ptr = std::shared_ptr<Window>;
		static Ptr create(const int& width, const int& height) { 
			return std::make_shared<Window>(width, height); 
		}

		Window(const int &width, const int &height);

		~Window();

		bool shouldClose();

		void pollEvents();

		auto getWindow() const { return m_window; }
		
	private:
		int m_width{ 0 };
		int m_height{ 0 };
		GLFWwindow* m_window{ NULL };
	};
}
