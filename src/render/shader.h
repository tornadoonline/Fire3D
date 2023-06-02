#pragma once

#include "base.h"
#include "device.h"

namespace Fire3D {

	class Shader {
	public:
		using Ptr = std::shared_ptr<Shader>;
		static Ptr create(const Device::Ptr& device, const std::string& fileName, VkShaderStageFlagBits shaderStage, const std::string& entryPoint) { 
			return std::make_shared<Shader>(device, fileName, shaderStage, entryPoint); 
		}

		Shader(const Device::Ptr& device, const std::string &fileName, VkShaderStageFlagBits shaderStage, const std::string &entryPoint);

		~Shader();

		auto getShaderStage() const { return mShaderStage; }
		auto& getShaderEntryPoint() const { return mEntryPoint; }
		auto getShaderModule() const { return mShaderModule; }

	private:
		VkShaderModule mShaderModule{ VK_NULL_HANDLE };
		Device::Ptr m_device{ nullptr };
		std::string mEntryPoint;
		VkShaderStageFlagBits mShaderStage;
	};
}