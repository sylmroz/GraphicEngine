#pragma once

#include <filesystem>

namespace GraphicEngine::Core
{
	class FileSystem
	{
	public:
		static std::filesystem::path getAssetPath();

		static std::filesystem::path getTexturePath();

		static std::filesystem::path getShaderPath();
		static std::filesystem::path getOpenGlShaderPath();
		static std::filesystem::path getVulkanShaderPath();
		static std::filesystem::path getHlslShaderPath();

		static std::filesystem::path getOpenGlShaderPath(const std::string& fileName);
		static std::filesystem::path getVulkanShaderPath(const std::string& fileName);
		static std::filesystem::path getHlslShaderPath(const std::string& fileName);

		static std::filesystem::path getModelsPath();

	private:
		static void init();

	private:
		static bool m_IsInitialized;
		static std::filesystem::path m_AssetPath;
	};
}