#ifndef GRAPHIC_ENGINE_COMMON_VERTEX_HPP
#define GRAPHIC_ENGINE_COMMON_VERTEX_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <map>
#include <vector>

namespace GraphicEngine::Common
{
	struct IVertex
	{
		virtual std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets() = 0;
	};

	struct VertexP : IVertex
	{
		glm::vec3 position = glm::vec3(0.0f);
		virtual std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets() override
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.push_back(std::pair(sizeof(position)/sizeof(position[0]), offsetof(VertexP, position)));
			return offsets;
		}
	};

	struct VertexPC : IVertex
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 color = glm::vec3(0.0f);
		virtual std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets() override
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(2);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPC, position)));
			offsets.push_back(std::pair(sizeof(color) / sizeof(color[0]), offsetof(VertexPCTc, color)));
			return offsets;
		}
	};

	struct VertexPTc : IVertex
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec2 texCoord = glm::vec2(0.0f);

		virtual std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets() override
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(2);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPTc, position)));
			offsets.push_back(std::pair(sizeof(texCoord) / sizeof(texCoord[0]), offsetof(VertexPTc, texCoord)));
			return offsets;
		}
	};

	struct VertexPCTc : IVertex
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 color = glm::vec3(0.0f);
		glm::vec2 texCoord = glm::vec2(0.0f);

		virtual std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets() override
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(3);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPCTc, position)));
			offsets.push_back(std::pair(sizeof(color) / sizeof(color[0]), offsetof(VertexPCTc, color)));
			offsets.push_back(std::pair(sizeof(texCoord) / sizeof(texCoord[0]), offsetof(VertexPCTc, texCoord)));
			return offsets;
		}
	};

	struct VertexPTcN : IVertex
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec2 texCoord = glm::vec2(0.0f);
		glm::vec3 normals = glm::vec3(0.0f);

		virtual std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets() override
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(3);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPTcN, position)));
			offsets.push_back(std::pair(sizeof(texCoord) / sizeof(texCoord[0]), offsetof(VertexPTcN, texCoord)));
			offsets.push_back(std::pair(sizeof(normals) / sizeof(normals[0]), offsetof(VertexPTcN, normals)));
			return offsets;
		}
	};

	struct VertexPTcNTB : IVertex
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec2 texCoord = glm::vec2(0.0f);
		glm::vec3 normals = glm::vec3(0.0f);
		glm::vec3 tangent = glm::vec3(0.0f);
		glm::vec3 bitangent = glm::vec3(0.0f);

		virtual std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets() override
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(5);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPTcNTB, position)));
			offsets.push_back(std::pair(sizeof(texCoord) / sizeof(texCoord[0]), offsetof(VertexPTcNTB, texCoord)));
			offsets.push_back(std::pair(sizeof(normals) / sizeof(normals[0]), offsetof(VertexPTcNTB, normals)));
			offsets.push_back(std::pair(sizeof(tangent) / sizeof(tangent[0]), offsetof(VertexPTcNTB, tangent)));
			offsets.push_back(std::pair(sizeof(bitangent) / sizeof(bitangent[0]), offsetof(VertexPTcNTB, bitangent)));
			return offsets;
		}
	};
}

#endif // !GRAPHIC_ENGINE_COMMON_VERTEX_HPP

