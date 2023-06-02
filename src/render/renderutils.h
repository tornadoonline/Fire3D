#pragma once
#include <vector>
#include <string>

#include <vulkan/vulkan.h>

namespace Fire3D{
    // validation layer callback
    VKAPI_ATTR VkBool32 VKAPI_PTR DebugUtilsMessengerCallbackEXT(
        VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
        const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
        void*                                            pUserData);

    VKAPI_ATTR VkResult VKAPI_PTR CreateDebugUtilsMessengerEXT(
        VkInstance instance, 
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
        const VkAllocationCallbacks* pAllocator, 
        VkDebugUtilsMessengerEXT* pMessenger);

    VKAPI_ATTR void VKAPI_PTR DestroyDebugUtilsMessengerEXT(
        VkInstance instance, 
        VkDebugUtilsMessengerEXT messenger, 
        const VkAllocationCallbacks* pAllocator);


    std::vector<char> readBinary(const std::string& fileName);
}