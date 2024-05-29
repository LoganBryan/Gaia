#pragma once
#define NOMINMAX
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

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

#include <chrono>

#include "Context.h"
#include "SwapChainManager.h"
#include "DeviceManager.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
const int MAX_FRAMES_IN_FLIGHT = 2;

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

private:
	VkPipelineLayout m_pipelineLayout;
	VkPipeline m_graphicsPipeline;

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