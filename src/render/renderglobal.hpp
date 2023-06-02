#pragma once
#include <vector>

namespace Fire3D{
    const bool g_enableValidationLayer = true;
    // instance & device
    const std::vector<const char*> g_validationLayerSupport = {"VK_LAYER_KHRONOS_validation"};
    // device
    const std::vector<const char*> g_requiredDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
}