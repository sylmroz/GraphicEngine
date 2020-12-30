#pragma once

#include "../../../Common/Vertex.hpp"

#include <string>
#include <memory>

namespace GraphicEngine::Engines::Graphic
{
	template <typename SkyboxGraphicPipelineImpl, template <typename> typename VertexBuffer, typename TextureCube, typename... Args>
	class SkyboxGraphicPipeline
	{
	public:		

        virtual void draw(Args... args) = 0;

        std::unique_ptr<VertexBuffer<Common::VertexP>> produceVertexBuffer(std::vector<Common::VertexP> skyBoxVertices)
        {
            return static_cast<SkyboxGraphicPipelineImpl*>(this)->produceVertexBuffer(skyBoxVertices);
        }
        std::shared_ptr<TextureCube> produceTextureCube(std::array<std::string, 6> faces)
        {
            return static_cast<SkyboxGraphicPipelineImpl*>(this)->produceTextureCube(faces);
        }

        void initialize(const std::string& basePath)
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

            m_cubeVertexBuffer = produceVertexBuffer(skyBoxVertices);
            m_textureCube = produceTextureCube(
                std::array<std::string, 6>{ {
                        basePath + "//px.png",
                            basePath + "//nx.png",
                            basePath + "//py.png",
                            basePath + "//ny.png",
                            basePath + "//pz.png",
                            basePath + "//nz.png"
                    }}
            );
        }

	protected:
		std::shared_ptr<TextureCube> m_textureCube;
		std::unique_ptr<VertexBuffer<Common::VertexP>> m_cubeVertexBuffer;
	};
}