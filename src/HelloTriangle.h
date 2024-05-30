#pragma once
#define NOMINMAX
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <glm/glm.hpp>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <cstring>
#include <fstream>
#include <optional>
#include <array>

#include <chrono>

#include "Context.h"
#include "SwapChainManager.h"
#include "DeviceManager.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
const int MAX_FRAMES_IN_FLIGHT = 2;

// Temp vertex data
struct Vertex
{
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

		// Position attribute
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		// Color attribute
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
};

const std::vector<Vertex> vertices =
{
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

// Could change to 32bit if we require more 
const std::vector<uint16_t> indicies =
{
	0, 1, 2, 2, 3, 0
};

class HelloTriangle
{
public:
	inline void Run()
	{
		CONTEXTMGR->InitWindow(WIDTH, HEIGHT);
		InitVulkan();
		MainLoop();
		Cleanup();
	}

private:
	void InitVulkan();
	void MainLoop();
	void Cleanup();

private:
	void DrawFrame();

	// Pipeline
	void CreateGraphicsPipeline();
	static std::vector<char> ReadFile(const std::string& filepath);
	VkShaderModule CreateShaderModule(const std::vector<char>& code);

	void CreateRenderPass();

	void CreateSyncObjects();

	void CreateCommandPool();
	void CreateCommandBuffers();
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void CreateVertexBuffer();
	void CreateIndexBuffer();

private:
	VkPipelineLayout m_pipelineLayout;
	VkPipeline m_graphicsPipeline;
	VkBuffer m_vertexBuffer;
	VkDeviceMemory m_vertexBufferMemory;	
	VkBuffer m_indexBuffer;
	VkDeviceMemory m_indexBufferMemory;

	VkCommandPool commandPool;

	// Sync objects
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	uint32_t currentFrame = 0;

	SwapChainManager* m_SwapChainManager;
	DeviceManager* m_DeviceManager;
};