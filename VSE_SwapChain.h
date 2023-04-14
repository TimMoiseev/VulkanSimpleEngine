#pragma once
#include "VSE_Device.h"
#include "VSE_DebugMessenger.h"

namespace vse {

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VseSwapChain {
    public:
        VseSwapChain(VseDevice& refDevice);
        ~VseSwapChain();
        VkExtent2D swapChainExtent; //размеры отрисовки изображения
    private:
        std::vector<VkImageView> swapChainImageViews;
        bool isSwapChainSuitable();
        void createImageViews();
        SwapChainSupportDetails querySwapChainSupport(); //возвращает в структуру поддерживаемы форматы, режимы смены кадров, макс. и мин. extent
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats); //выбор формата отображения(цветовое пространство и т.д.)
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes); //режим смены кадров (fifo, mailbox,...)
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities); // выбор размера отрисовываемой области
        VseDevice& refDevice;
        VkSwapchainKHR swapChain; 
        std::vector<VkImage> swapChainImages; //контейнер для хранения изображений рендринга
        VkFormat swapChainImageFormat;
        
    };
}