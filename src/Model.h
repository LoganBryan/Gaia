#pragma once
#include <vulkan/vulkan.h>
#include <string>

namespace GAIA
{
	class Model
	{
	public:
		void LoadFromFile(std::string filepath); // DeviceManager, VmaUsage
		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

	private:
		VkBuffer m_vertexBuffer;
		VkBuffer m_indexbuffer;
		uint32_t m_vertexCount;
		uint32_t m_indexCount;
	};
}

