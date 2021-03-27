#pragma once

#include "../../Engines/Graphic/Shaders/Models/Material.hpp"
#include <string>
#include <variant>

namespace GraphicEngine::Scene
{
	struct MeshMaterialTexturePaths
	{
		std::string ambient;
		std::string diffuse;
		std::string specular;
		float shinnes;
	};
	struct MeshMaterial
	{
		glm::vec4 solidColor;
		std::variant<Engines::Graphic::Shaders::Material, MeshMaterialTexturePaths> baseMaterial;
	};
}