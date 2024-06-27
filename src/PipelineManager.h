#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <map>

namespace GAIA
{
	class PipelineManager
	{
	public:
		void Init(); // DeviceManager, SwapChainManager
		VkPipeline& CreateGraphicsPipeline(std::string name); // PipelineConfig
		VkPipeline& GetPipeline(std::string name);
		void Cleanup();

	private:
		std::map<std::string, VkPipeline> m_pipelines;
		std::map<std::string, VkPipelineLayout> m_pipelineLayouts;
	};
}

