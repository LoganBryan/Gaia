#pragma once
#ifndef HELLO_TRIANGLE_H
#define HELLO_TRIANGLE_H

#define NOMINMAX
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <cstring>
#include <fstream>
#include <optional>
#include <array>

#include <chrono>
#include <stb_image.h>
#include <tiny_obj_loader.h>

#include "Context.h"
#include "SwapChainManager.h"
#include "DeviceManager.h"
#include "VmaUsage.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
const int MAX_FRAMES_IN_FLIGHT = 2;

const std::string MODEL_PATH = "assets/models/monkey.obj";
const std::string TEXTURE_PATH = "assets/textures/thisguy.png";

// Temp vertex data
struct Vertex
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

		// Position attribute
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		// Color attribute
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		// Texture attribute
		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}

	inline bool operator==(const Vertex& other) const
	{
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}
};

// Implement a hash function for vertex
namespace std
{
	template<> struct hash<Vertex>
	{
		size_t operator()(Vertex const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}
struct UniformBufferObject
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
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
	void DrawFrame();

private:

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
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VmaAllocation& allocation);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

	void CreateVertexBuffer();
	void CreateIndexBuffer();

	// Uniform buffer
	void CreateDescriptorSetLayout();
	void CreateUniformBuffers();
	void UpdateUniformBuffer(uint32_t currentImage);
	void CreateDescriptorPool();
	void CreateDescriptorSets();

	// Depth Buffer
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat FindDepthFormat();
	bool HasStencilComponent(VkFormat format);

	void CreateDepthResources();

	// Texture mapping -- Right now this works synchronously but should be changed to be asynchrnous
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

	void CreateTextureImage();
	void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlags, VkMemoryPropertyFlags properties, VkImage& image, VmaAllocation& allocation);
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void CreateTextureImageView();
	void CreateTextureSampler();

	void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t width, int32_t height, uint32_t mipLevels);

	// Model Loading
	void LoadModel();

	// Multisampler
	void CreateColorResources();

private:

	VkDescriptorSetLayout m_descriptorSetLayout;
	VkPipelineLayout m_pipelineLayout;
	VkPipeline m_graphicsPipeline;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	VkBuffer m_vertexBuffer;
	VmaAllocation m_vertexBufferMemory;

	VkBuffer m_indexBuffer;
	VmaAllocation m_indexBufferMemory;

	uint32_t m_mipLevels;

	VkImage m_textureImage;
	VmaAllocation m_textureImageMemory;
	VkImageView m_textureImageView;
	VkSampler m_textureSampler;

	VkImage m_depthImage;
	VmaAllocation m_depthImageMemory;
	VkImageView m_depthImageView;

	VkImage m_colorImage;
	VmaAllocation m_colorImageMemory;
	VkImageView m_colorImageView;

	std::vector<VkBuffer> m_uniformBuffers;
	std::vector<VmaAllocation> m_uniformBuffersMemory;
	std::vector<void*> m_uniformBuffersMapped;

	VkDescriptorPool m_descriptorPool;
	std::vector<VkDescriptorSet> m_descriptorSets;

	VkCommandPool m_commandPool;

	// Sync objects
	std::vector<VkCommandBuffer> m_commandBuffers;
	std::vector<VkSemaphore> m_imageAvailableSemaphores;
	std::vector<VkSemaphore> m_renderFinishedSemaphores;
	std::vector<VkFence> m_inFlightFences;
	uint32_t m_currentFrame = 0;

	SwapChainManager* m_SwapChainManager;
	DeviceManager* m_DeviceManager;
};

#endif // HELLO_TRIANGLE_H