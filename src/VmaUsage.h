#pragma once

#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"
#include <mutex>

#include "Singleton.h"
namespace GAIA
{
	class VmaUsage
	{
	public:
		void CreateAllocator(VkInstance instance); // VkInstance instance, DeviceManager deviceManager -- Could also make deviceManager a singleton?
		VkBuffer AllocateBuffer(VkBufferCreateInfo bufferInfo, VmaAllocationCreateInfo allocInfo);
		VkImage AllocateImage(VkImageCreateInfo imageInfo, VmaAllocationCreateInfo allocInfo);
		void Cleanup();

	private:
		VmaAllocator m_allocator;
	};
}