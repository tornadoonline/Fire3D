#include "renderutils.h"
#include <exception>
#include <fstream>
#include <iostream>
namespace Fire3D{
    // validation layer callback
    VKAPI_ATTR VkBool32 VKAPI_PTR DebugUtilsMessengerCallbackEXT(
        VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
        const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
        void*                                            pUserData)
    {
        std::cout << "DebugUtilsMessenger: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

    VKAPI_ATTR VkResult VKAPI_PTR CreateDebugUtilsMessengerEXT(
        VkInstance instance, 
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
        const VkAllocationCallbacks* pAllocator, 
        VkDebugUtilsMessengerEXT* pMessenger)
    {
        auto _func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if(_func) {
            return _func(instance, pCreateInfo, pAllocator, pMessenger);
        }
        else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    VKAPI_ATTR void VKAPI_PTR DestroyDebugUtilsMessengerEXT(
        VkInstance instance, 
        VkDebugUtilsMessengerEXT messenger, 
        const VkAllocationCallbacks* pAllocator)
    {
        auto _func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if(_func) {
            _func(instance, messenger, pAllocator);
        }
    }

    std::vector<char> readBinary(const std::string& fileName)
    {
        std::ifstream _file(fileName.c_str(), std::ios::ate | std::ios::binary | std::ios::in);
        if(!_file){
            throw std::exception("Failed To Open File.");
        }
        auto _fileSize = _file.tellg();
        std::vector<char> _buffer(_fileSize);
        _file.seekg(0);
        _file.read(_buffer.data(), _fileSize);
        _file.close();
        return _buffer;
    }

}