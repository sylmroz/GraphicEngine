#pragma once

#include "../../../Common/Vertex.hpp"

#include <string>
#include <memory>

namespace GraphicEngine::Engines::Graphic
{
	template <template <typename> typename VertexBuffer, typename TextureCube, typename... Args>
	class SkyboxGraphicPipeline
	{
	public:
		SkyboxGraphicPipeline(const std::string& basePath)
		{
            std::vector<Common::VertexP> skyBoxVertices =
            { {
                glm::vec3{ -1.0f,  1.0f, -1.0f },
                glm::vec3{ -1.0f, -1.0f, -1.0f },
                glm::vec3{ 1.0f, -1.0f, -1.0f },
                glm::vec3{ 1.0f, -1.0f, -1.0f },
                glm::vec3{ 1.0f,  1.0f, -1.0f },
                glm::vec3{ -1.0f,  1.0f, -1.0f },

                glm::vec3{ -1.0f, -1.0f,  1.0f },
                glm::vec3{ -1.0f, -1.0f, -1.0f },
                glm::vec3{ -1.0f,  1.0f, -1.0f },
                glm::vec3{ -1.0f,  1.0f, -1.0f },
                glm::vec3{ -1.0f,  1.0f,  1.0f },
                glm::vec3{ -1.0f, -1.0f,  1.0f },

                glm::vec3{ 1.0f, -1.0f, -1.0f },
                glm::vec3{ 1.0f, -1.0f,  1.0f },
                glm::vec3{ 1.0f,  1.0f,  1.0f },
                glm::vec3{ 1.0f,  1.0f,  1.0f },
                glm::vec3{ 1.0f,  1.0f, -1.0f },
                glm::vec3{ 1.0f, -1.0f, -1.0f },

                glm::vec3{ -1.0f, -1.0f,  1.0f },
                glm::vec3{ -1.0f,  1.0f,  1.0f },
                glm::vec3{ 1.0f,  1.0f,  1.0f },
                glm::vec3{ 1.0f,  1.0f,  1.0f },
                glm::vec3{ 1.0f, -1.0f,  1.0f },
                glm::vec3{ -1.0f, -1.0f,  1.0f },

                glm::vec3{ -1.0f,  1.0f, -1.0f },
                glm::vec3{ 1.0f,  1.0f, -1.0f },
                glm::vec3{ 1.0f,  1.0f,  1.0f },
                glm::vec3{ 1.0f,  1.0f,  1.0f },
                glm::vec3{ -1.0f,  1.0f,  1.0f },
                glm::vec3{ -1.0f,  1.0f, -1.0f },

                glm::vec3{ -1.0f, -1.0f, -1.0f },
                glm::vec3{ -1.0f, -1.0f,  1.0f },
                glm::vec3{ 1.0f, -1.0f, -1.0f },
                glm::vec3{ 1.0f, -1.0f, -1.0f },
                glm::vec3{ -1.0f, -1.0f,  1.0f },
                glm::vec3{ 1.0f, -1.0f,  1.0f }
            } };

            m_cubeVertexBuffer = std::make_unique<VertexBuffer<Common::VertexP>>(skyBoxVertices);
            m_textureCube = std::make_unique<TextureCube>(
                std::array<std::string, 6>{ {
                        basePath + "//px.png",
                            basePath + "//nx.png",
                            basePath + "//py.png",
                            basePath + "//ny.png",
                            basePath + "//pz.png",
                            basePath + "//nz.png"
                    }}
            );
        };

        virtual void draw(Args... args) = 0;

	protected:
		std::unique_ptr<TextureCube> m_textureCube;
		std::unique_ptr<VertexBuffer<Common::VertexP>> m_cubeVertexBuffer;
	};
}