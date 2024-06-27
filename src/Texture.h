#pragma once
#include <vulkan/vulkan.h>
#include <string>

namespace GAIA
{
	class Texture
	{
	public:
		void LoadFromFile(std::string filepath); // DeviceManager, VmaUsage
		void Bind(VkCommandBuffer commandBuffer, VkDescriptorSet descriptorSet);

	private:
		VkImage m_image;
		VkImageView m_imageView;
		VkSampler m_sampler;
	};
}

