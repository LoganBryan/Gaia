#pragma once
namespace GAIA
{
	class ResourceManager
	{
	public:
		void Init(); // DeviceManager and VmaUsage (if not singletons)
		// Model& LoadModel(filepath)
		// Texture& LoadTexture(filepath)
		// Model& GetModel(name)
		// Texture& GetTexture(name)
		void Cleanup();

	private:
		// DeviceManager
		// VmaUsage (only if they're both not singletons - still unsure)
		// Map of Models
		// Map of Textures
	};
}
