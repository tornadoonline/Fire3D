#pragma once

#include "base.h"

namespace Fire3D {
	
	class Instance {
	public:
		using Ptr = std::shared_ptr<Instance>;
		static Ptr create() { return std::make_shared<Instance>(); }

		Instance();

		~Instance();

		void printAvailableExtensions();

		std::vector<const char*> getRequiredExtensions();

		//layers
		bool checkValidationLayerSupport();
		void setupDebugger();

		auto getInstance()->VkInstance { return m_instance; }

	private:
		VkInstance m_instance{ VK_NULL_HANDLE };
		VkDebugUtilsMessengerEXT m_debugUtilsMessengerEXT{ VK_NULL_HANDLE };
	};
}