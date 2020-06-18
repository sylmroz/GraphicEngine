#include "FileSystem.hpp"
#include "../Configuration.hpp"

bool GraphicEngine::Core::FileSystem::m_IsInitialized = false;
std::filesystem::path GraphicEngine::Core::FileSystem::m_AssetPath = "";

void GraphicEngine::Core::FileSystem::init()
{
    if (!m_IsInitialized)
    {
        Configuration cfg;
        m_AssetPath = cfg.getProperty<std::string>("paths:assets");
        //if (m_AssetPath.empty() || std::filesystem::exists(m_AssetPath))
        //{
          //  throw std::runtime_error("Cannot find Assets path!");
        //}
    }
}

std::filesystem::path GraphicEngine::Core::FileSystem::getAssetPath()
{
    init();
    return m_AssetPath;
}

std::filesystem::path GraphicEngine::Core::FileSystem::getTexturePath()
{
    std::filesystem::path path = getAssetPath() / "Images";
    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error("Path to texture does not exist!");
    }
    return path;
}

std::filesystem::path GraphicEngine::Core::FileSystem::getShaderPath()
{
    std::filesystem::path path = getAssetPath() / "Shaders";
    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error("Path to shaders does not exist!");
    }
    return path;
}

std::filesystem::path GraphicEngine::Core::FileSystem::getOpenGlShaderPath()
{
    std::filesystem::path path = getShaderPath() / "glsl";
    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error("Path to glsl shaders does not exist!");
    }
    return path;
}

std::filesystem::path GraphicEngine::Core::FileSystem::getVulkanShaderPath()
{
    std::filesystem::path path = getShaderPath() / "spv";
    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error("Path to vulkan shaders does not exist!");
    }
    return path;
}

std::filesystem::path GraphicEngine::Core::FileSystem::getHlslShaderPath()
{
    std::filesystem::path path = getShaderPath() / "hlsl";
    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error("Path to hlsl shaders does not exist!");
    }
    return path;
}

std::filesystem::path GraphicEngine::Core::FileSystem::getOpenGlShaderPath(const std::string& fileName)
{
    std::filesystem::path path = getOpenGlShaderPath() / fileName;
    if (!std::filesystem::exists(path))
    {
        std::string err = "File " + fileName + " does not exist!";
        throw std::runtime_error(err);
    }
    return path;
}

std::filesystem::path GraphicEngine::Core::FileSystem::getVulkanShaderPath(const std::string& fileName)
{
    std::filesystem::path path = getVulkanShaderPath() / fileName;
    if (!std::filesystem::exists(path))
    {
        std::string err = "File " + fileName + " does not exist!";
        throw std::runtime_error(err);
    }
    return path;
}

std::filesystem::path GraphicEngine::Core::FileSystem::getHlslShaderPath(const std::string& fileName)
{
    std::filesystem::path path = getHlslShaderPath() / fileName;
    if (!std::filesystem::exists(path))
    {
        std::string err = "File " + fileName + " does not exist!";
        throw std::runtime_error(err);
    }
    return path;
}

std::filesystem::path GraphicEngine::Core::FileSystem::getModelsPath()
{
    std::filesystem::path path = getAssetPath() / "Models";
    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error("Path to models does not exist!");
    }
    return path;
}
