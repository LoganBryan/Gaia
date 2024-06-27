#pragma once
#ifndef VMA_USAGE_H
#define VMA_USAGE_H

#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"
#include <mutex>

#include "Singleton.h"

namespace GAIA
{
	class VmaUsage : public Singleton<VmaUsage>
	{
	public:
		void CreateAllocator(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device);
		void DestroyAllocator();

		inline VmaAllocator GetAllocator() { return m_allocator; }

		void MapAndCopyBuffer(VmaAllocation allocation, const void* src, size_t size);

	private:
		VkInstance m_instance;
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_device;

		VmaAllocator m_allocator;

	private:
		friend class Singleton<VmaUsage>;
		VmaUsage() : m_allocator(nullptr) {}
		~VmaUsage() {}
	};
}

#define VMAUSG GAIA::VmaUsage::GetInstance()

#endif // VMA_USAGE_H