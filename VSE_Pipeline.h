#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include "VSE_SwapChain.h"
namespace vse {
	class VsePipeline {
	public:
		VsePipeline(VseDevice& refDevice, VseSwapChain& refSwapChain);
		~VsePipeline();
		void drawFrame();
	private:
		void createSyncObject();
		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;
		VkFence inFlightFence;
		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		VseSwapChain& refSwapChain;
		VkCommandBuffer commandBuffer;
		VkCommandPool commandPool;
		void createFramebuffers();
		void createCommandPool();
		void createCommandBuffers();
		static std::vector<char> readFile(const std::string& filename);
		VseDevice& refDevice;
		const VkExtent2D& extent;
		const VkFormat& swapChainmageFormat;
		VkShaderModule createShaderModule(const std::vector<char>& code);
		void createGraphicsPipeline();
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;
		void createRenderPass();
		VkRenderPass renderPass;
	};
}