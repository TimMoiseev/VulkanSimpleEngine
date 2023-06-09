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
        VkExtent2D getSwapChainExtent();
        VkFormat getSwapChainImageFormat();
        VkSwapchainKHR& getSwapChain();
        void createFrameBuffer(VkRenderPass& renderPass);
        std::vector<VkFramebuffer> swapChainFramebuffers;
        std::vector<VkImageView> swapChainImageViews;
    private:
        VkExtent2D swapChainExtent; //������� ��������� �����������
        
        bool isSwapChainSuitable();
        void createImageViews();
        SwapChainSupportDetails querySwapChainSupport(); //���������� � ��������� ������������� �������, ������ ����� ������, ����. � ���. extent
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats); //����� ������� �����������(�������� ������������ � �.�.)
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes); //����� ����� ������ (fifo, mailbox,...)
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities); // ����� ������� �������������� �������
        VseDevice& refDevice;
        VkSwapchainKHR swapChain; 
        std::vector<VkImage> swapChainImages; //��������� ��� �������� ����������� ���������
        VkFormat swapChainImageFormat;
        
    };
}