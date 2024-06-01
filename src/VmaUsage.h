#pragma once
#ifndef VMA_USAGE_H
#define VMA_USAGE_H

#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"
#include <mutex>

namespace GAIA
{
	class VmaUsage
	{
	public:

		void CreateAllocator(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device);
		void DestroyAllocator();

		inline VmaAllocator GetAllocator() { return m_allocator; }

		void MapAndCopyBuffer(VmaAllocation allocation, const void* src, size_t size);
	public:
		// Assure class can't be cloned or assigned
		VmaUsage(VmaUsage& other) = delete;
		void operator=(const VmaUsage&) = delete;

		static inline VmaUsage* Get()
		{
			std::lock_guard<std::mutex> lock(mutex);
			if (pInstance == nullptr)
			{
				pInstance = new VmaUsage();
			}
			return pInstance;
		}
	private:
		static VmaUsage* pInstance;
		static std::mutex mutex;

		VkInstance m_instance;
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_device;

		VmaAllocator m_allocator;
	protected:
		VmaUsage() {}
		~VmaUsage() {}
	};
}

#define VMAUSG (GAIA::VmaUsage::Get())

#endif // VMA_USAGE_H