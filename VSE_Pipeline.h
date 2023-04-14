#include <vulkan/vulkan.h>
#include <vector>
#include <string>
namespace vse {
	class VsePipeline {
	public:
		VsePipeline(VkDevice& refDevice, VkExtent2D& extent);
		~VsePipeline();
	private:
		static std::vector<char> readFile(const std::string& filename);
		VkDevice& refDevice;
		const VkExtent2D& extent;
		VkShaderModule createShaderModule(const std::vector<char>& code);
		void createGraphicsPipeline();
		VkPipelineLayout pipelineLayout;
	};
}