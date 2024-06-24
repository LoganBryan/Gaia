#pragma once
#ifndef BUFFER_H
#define BUFFER_H

#include <vulkan/vulkan.h>
#include "DeviceManager.h"
#include "VmaUsage.h"

#include "Singleton.h"

namespace GAIA
{
	class Buffer : public Singleton<Buffer>
	{
	public:
		void Create(VkPhysicalDevice physicalDevice, VkDevice device) { m_physicalDevice = physicalDevice, m_device = device; };
		void Clean() {};

		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VmaAllocation& allocation);
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkQueue queue, VkCommandPool commandPool);

		void CreateCommandPool();
		void CreateCommandBuffers();
		void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VkRenderPass renderPass, std::vector<VkFramebuffer>& frameBuffers, VkExtent2D extents, VkPipeline pipeline, VkPipelineLayout pipelineLayout);

		void CreateVertexBuffer(VkQueue queue, VkCommandPool commandPool);
		void CreateIndexBuffer(VkQueue queue, VkCommandPool commandPool);

		VkCommandBuffer BeginSingleTimeCommands(VkCommandPool commandPool);
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue queue, VkCommandPool commandPool);

	private:
		friend class Singleton<Buffer>;

		Buffer() {}
		~Buffer() {}

	private:
		void Recreate() {};

		VkPhysicalDevice m_physicalDevice;
		VkDevice m_device;

		VkCommandPool m_commandPool;
		std::vector<VkCommandBuffer> m_commandBuffers;

		VkBuffer m_vertexBuffer;
		VmaAllocation m_vertexBufferMemory;

		VkBuffer m_indexBuffer;
		VmaAllocation m_indexBufferMemory;

		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
	};
}

#define BUFINST GAIA::Buffer::GetInstance()

#endif // !BUFFER_H