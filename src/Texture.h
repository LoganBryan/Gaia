#pragma once

#include <vulkan/vulkan.h>
#include <cstdint>
#include <stb_image.h>

#include "Context.h"
#include "VmaUsage.h"
#include "Buffers.h"
#include "ImageUtils.h"

class Texture
{
public:
	void Init(VkDevice device, VkPhysicalDevice physicalDevice) { m_device = device; m_physDevice = physicalDevice; }

	void CreateTextureImage(VkCommandPool commandPool, VkQueue queue);

	inline VkImageView CreateTextureImageView() { return m_textureImageView = ImageUtils::CreateImageView(m_device, m_textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels); }
	void CreateTextureSampler();

	void GenerateMipmaps(VkCommandPool commandPool, VkQueue queue, VkImage image, VkFormat imageFormat, int32_t width, int32_t height, uint32_t mipLevels);
private:
	VkDevice m_device;
	VkPhysicalDevice m_physDevice;

	VkImage m_textureImage;
	VmaAllocation m_textureImageMemory;
	VkImageView m_textureImageView;
	VkSampler m_textureSampler;

	uint32_t m_mipLevels;
};

