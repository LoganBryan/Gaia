#include "VmaUsage.h"
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1

// Static Members
GAIA::VmaUsage* GAIA::VmaUsage::pInstance;
std::mutex GAIA::VmaUsage::mutex;

void GAIA::VmaUsage::CreateAllocator(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device)
{
	if (m_allocator)
	{
		printf("Allocator already created!");
		return;
	}

	VmaVulkanFunctions vulkanFunctions = {};
	vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
	vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

	VmaAllocatorCreateInfo allocCreateInfo = {};
	allocCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
	allocCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
	allocCreateInfo.physicalDevice = physicalDevice;
	allocCreateInfo.device = device;
	allocCreateInfo.instance = instance;
	allocCreateInfo.pVulkanFunctions = &vulkanFunctions;

	if (vmaCreateAllocator(&allocCreateInfo, &m_allocator) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create VMA allocator!");
	}
}

void GAIA::VmaUsage::DestroyAllocator()
{
	if (m_allocator)
	{
		vmaDestroyAllocator(m_allocator);
	}
}

void GAIA::VmaUsage::MapAndCopyBuffer(VmaAllocation allocation, const void* src, size_t size)
{
	void* data;
	vmaMapMemory(m_allocator, allocation, &data);
	memcpy(data, src, size);
	vmaUnmapMemory(m_allocator, allocation);
}


// VMAUSG->MapAndCopyBuffer(stagingBufferMemory, pixels, static_cast<size_t>(imageSize));