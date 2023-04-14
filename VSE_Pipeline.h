
#include <vulkan/vulkan.h>
#include <vector>
namespace vse {
	class VsePipeline {
	public:
		VsePipeline(VkDevice& refDevice);
		~VsePipeline();
	private:
		static std::vector<char> readFile(const std::string& filename);
		VkDevice& refDevice;
		VkShaderModule createShaderModule(const std::vector<char>& code);
		void createGraphicsPipeline();
	};
}