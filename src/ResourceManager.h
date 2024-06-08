#pragma once
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include "VmaUsage.h"

class ResourceManager
{
public:

	void Init(VkDevice device) { m_device = device; };

	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VmaAllocation& allocation);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

private:
	VkDevice m_device;

	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
};

