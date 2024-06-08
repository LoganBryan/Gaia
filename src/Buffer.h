#pragma once
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include "DeviceManager.h"

class Buffer
{
public:
	Buffer() = default;

	virtual void Create() {};
	virtual void Clean() {};

protected:
	virtual void Recreate() {};


};