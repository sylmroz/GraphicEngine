#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>

namespace GraphicEngine::Common
{
	enum VertexType
	{
		Position = 0x0000001,
		Color = 0x0000002,
		TexCoord = 0x0000004,
		Normal = 0x0000008,
		Tangent = 0x0000010,
		BiTangent = 0x0000020,
		Bone = 0x0000040,
		Weight = 0x0000080,
	};

	struct VertexP
	{
		VertexP() {}
		VertexP(glm::vec3 pos)
		{
			position = pos;
		}

		glm::vec3 position = glm::vec3(0.0f);

		static std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets()
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexP, position)));
			return offsets;
		}

		static uint32_t getStride()
		{
			return sizeof(VertexP);
		}

		static int getType()
		{
			return VertexType::Position;
		}
	};

	struct VertexPC
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 color = glm::vec3(0.0f);

		static std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets()
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(2);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPC, position)));
			offsets.push_back(std::pair(sizeof(color) / sizeof(color[0]), offsetof(VertexPC, color)));
			return offsets;
		}

		static uint32_t getStride()
		{
			return sizeof(VertexPC);
		}

		static int getType()
		{
			return (VertexType::Position | VertexType::Color);
		}
	};

	struct VertexPN
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 normal = glm::vec3(0.0f);

		static std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets()
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(2);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPN, position)));
			offsets.push_back(std::pair(sizeof(normal) / sizeof(normal[0]), offsetof(VertexPN, normal)));
			return offsets;
		}

		static uint32_t getStride()
		{
			return sizeof(VertexPC);
		}

		static int getType()
		{
			return (VertexType::Position | VertexType::Normal);
		}
	};

	struct VertexPTc
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec2 texCoord = glm::vec2(0.0f);

		static std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets()
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(2);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPTc, position)));
			offsets.push_back(std::pair(sizeof(texCoord) / sizeof(texCoord[0]), offsetof(VertexPTc, texCoord)));
			return offsets;
		}

		static uint32_t getStride()
		{
			return sizeof(VertexPTc);
		}

		static int getType()
		{
			return (VertexType::Position | VertexType::TexCoord);
		}
	};

	struct VertexPCTc
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 color = glm::vec3(0.0f);
		glm::vec2 texCoord = glm::vec2(0.0f);

		static std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets()
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(3);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPCTc, position)));
			offsets.push_back(std::pair(sizeof(color) / sizeof(color[0]), offsetof(VertexPCTc, color)));
			offsets.push_back(std::pair(sizeof(texCoord) / sizeof(texCoord[0]), offsetof(VertexPCTc, texCoord)));
			return offsets;
		}

		static uint32_t getStride()
		{
			return sizeof(VertexPCTc);
		}

		static int getType()
		{
			return (VertexType::Position | VertexType::Color | VertexType::TexCoord);
		}
	};

	struct VertexPTcN
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec2 texCoord = glm::vec2(0.0f);
		glm::vec3 normal = glm::vec3(0.0f);

		static std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets()
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(3);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPTcN, position)));
			offsets.push_back(std::pair(sizeof(texCoord) / sizeof(texCoord[0]), offsetof(VertexPTcN, texCoord)));
			offsets.push_back(std::pair(sizeof(normal) / sizeof(normal[0]), offsetof(VertexPTcN, normal)));
			return offsets;
		}

		static uint32_t getStride()
		{
			return sizeof(VertexPTcN);
		}

		static int getType()
		{
			return (VertexType::Position | VertexType::TexCoord | VertexType::Normal);
		}
	};

	struct VertexPTcNTB
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec2 texCoord = glm::vec2(0.0f);
		glm::vec3 normal = glm::vec3(0.0f);
		glm::vec3 tangent = glm::vec3(0.0f);
		glm::vec3 bitangent = glm::vec3(0.0f);

		static std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets()
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(5);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPTcNTB, position)));
			offsets.push_back(std::pair(sizeof(texCoord) / sizeof(texCoord[0]), offsetof(VertexPTcNTB, texCoord)));
			offsets.push_back(std::pair(sizeof(normal) / sizeof(normal[0]), offsetof(VertexPTcNTB, normal)));
			offsets.push_back(std::pair(sizeof(tangent) / sizeof(tangent[0]), offsetof(VertexPTcNTB, tangent)));
			offsets.push_back(std::pair(sizeof(bitangent) / sizeof(bitangent[0]), offsetof(VertexPTcNTB, bitangent)));
			return offsets;
		}

		static uint32_t getStride()
		{
			return sizeof(VertexPTcNTB);
		}

		static int getType()
		{
			return (VertexType::Position | VertexType::TexCoord | VertexType::Normal | VertexType::Tangent | VertexType::BiTangent);
		}
	};
}
