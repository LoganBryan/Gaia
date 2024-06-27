#pragma once
#ifndef CONTEXT_H
#define CONTEXT_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <optional>
#include <mutex>
#include <iostream>
#include <stdexcept>

#include "Singleton.h"

// Temp until multiple models/ textures get supported
const std::string MODEL_PATH = "assets/models/monkey.obj";
const std::string TEXTURE_PATH = "assets/textures/mapDist.png";

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
const int MAX_FRAMES_IN_FLIGHT = 2;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif // NDEBUG

//const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation", "VK_LAYER_LUNARG_api_dump"}; // If I want to dump to a text file (NOT A GREAT IDEA AS IT FILLS UP FAST)
const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation"};
const std::vector<const char*> deviceExtensions = { "VK_KHR_swapchain"};
const std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

struct QueueFamilyIndicies
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool IsComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

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

namespace GAIA
{
	class Context : public Singleton<Context>
	{
	public:
		void InitWindow(int width, int height);
		void CreateSurface();
		void CreateInstance();
		void CleanupInstance();
		VkInstance GetVkInstance() const { return m_VkInstance; }

		void SetupDebugMessenger();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		void SetupEnvironmentVariables();

	public:

		QueueFamilyIndicies& FindQueueFamilies(VkPhysicalDevice device);

		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		std::vector<const char*> GetRequiredExtensions();
		bool CheckValidationLayerSupport();

		inline VkSurfaceKHR GetSurface() { return m_surface; }
		inline bool GetFrameResized() { return m_frameBufferResized; }
		inline GLFWwindow* GetWindow() { return m_window; }

		inline void SetFrameResized(bool state) { m_frameBufferResized = state; }

	private:
		static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height);

		VkDebugUtilsMessengerEXT m_debugMessenger;

		// Messenger
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

		// Debug messenger to be passed into this object
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	private:
		friend class Singleton<Context>;

		Context() {}
		~Context() {}

		GLFWwindow* m_window;

		// Window surface
		VkSurfaceKHR m_surface;

		VkInstance m_VkInstance;

		bool m_frameBufferResized = false;

	};
}

#define CONTEXTMGR GAIA::Context::GetInstance()

#endif // CONTEXT_H