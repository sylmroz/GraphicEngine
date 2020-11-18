#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>

#include "../Core/Utils/TupleUtils.hpp"

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

	// Position
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

	// Position, Weight
	struct VertexPW : VertexP
	{
		VertexPW() = default;
		VertexPW(glm::vec3 position, glm::vec4 weight) :
			VertexP{position}, weight{weight} {}

		glm::vec4 weight = glm::vec4(0.0f);

		static std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets()
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(2);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPW, position)));
			offsets.push_back(std::pair(sizeof(weight) / sizeof(weight[0]), offsetof(VertexPW, weight)));
			return offsets;
		}

		static uint32_t getStride()
		{
			return sizeof(VertexPW);
		}

		static int getType()
		{
			return (VertexType::Position | VertexType::Weight);
		}
	};

	// Position, Color
	struct VertexPC : VertexP
	{
		VertexPC() {}
		VertexPC(glm::vec3 position, glm::vec3 color) :
			VertexP(position), color(color) {}

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

	// Position, Color, Weight
	struct VertexPCW : VertexPC
	{
		VertexPCW() = default;
		VertexPCW(glm::vec3 position, glm::vec3 color, glm::vec4 weight) :
			VertexPC{ position, color }, weight{ weight } {}

		glm::vec4 weight = glm::vec4(0.0f);

		static std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets()
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(3);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPCW, position)));
			offsets.push_back(std::pair(sizeof(color) / sizeof(color[0]), offsetof(VertexPCW, color)));
			offsets.push_back(std::pair(sizeof(weight) / sizeof(weight[0]), offsetof(VertexPCW, weight)));
			return offsets;
		}

		static uint32_t getStride()
		{
			return sizeof(VertexPCW);
		}

		static int getType()
		{
			return (VertexType::Position | VertexType::Color | VertexType::Weight);
		}
	};

	// Position, Normal
	struct VertexPN : VertexP
	{
		VertexPN() {}
		VertexPN(glm::vec3 position, glm::vec3 normal) :
			VertexP(position), normal(normal) {}

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
			return sizeof(VertexPN);
		}

		static int getType()
		{
			return (VertexType::Position | VertexType::Normal);
		}
	};

	// Position, Normal, Weight
	struct VertexPNW : VertexPN
	{
		VertexPNW() = default;
		VertexPNW(glm::vec3 position, glm::vec3 normal, glm::vec4 weight) :
			VertexPN{ position, normal }, weight{ weight } {}

		glm::vec4 weight = glm::vec4(0.0f);

		static std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets()
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(3);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPNW, position)));
			offsets.push_back(std::pair(sizeof(normal) / sizeof(normal[0]), offsetof(VertexPNW, normal)));
			offsets.push_back(std::pair(sizeof(weight) / sizeof(weight[0]), offsetof(VertexPNW, weight)));
			return offsets;
		}

		static uint32_t getStride()
		{
			return sizeof(VertexPNW);
		}

		static int getType()
		{
			return (VertexType::Position | VertexType::Normal | VertexType::Weight);
		}
	};

	// Position, Texture Coordinates
	struct VertexPTc : VertexP
	{
		VertexPTc() {}
		VertexPTc(glm::vec3 position, glm::vec2 texCoord) :
			VertexP(position), texCoord(texCoord) {}

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

	// Position, Texture Coordinates, Weight
	struct VertexPTcW : VertexPTc
	{
		VertexPTcW() = default;
		VertexPTcW(glm::vec3 position, glm::vec2 texCoord, glm::vec4 weight) :
			VertexPTc{ position, texCoord }, weight{ weight } {}

		glm::vec4 weight = glm::vec4(0.0f);

		static std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets()
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(3);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPTcW, position)));
			offsets.push_back(std::pair(sizeof(texCoord) / sizeof(texCoord[0]), offsetof(VertexPTcW, texCoord)));
			offsets.push_back(std::pair(sizeof(weight) / sizeof(weight[0]), offsetof(VertexPTcW, weight)));
			return offsets;
		}

		static uint32_t getStride()
		{
			return sizeof(VertexPTcW);
		}

		static int getType()
		{
			return (VertexType::Position | VertexType::TexCoord | VertexType::Weight);
		}
	};

	// Position, Color, Texture Coordinates
	struct VertexPCTc : VertexPC
	{
		VertexPCTc() {}
		VertexPCTc(glm::vec3 position, glm::vec3 color, glm::vec2 texCoord) :
			VertexPC(position, color), texCoord(texCoord) {}

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

	// Position, Color, Texture Coordinates, Weights
	struct VertexPCTcW : VertexPCTc
	{
		VertexPCTcW() = default;
		VertexPCTcW(glm::vec3 position, glm::vec3 color, glm::vec2 texCoord, glm::vec4 weight) :
			VertexPCTc{position, color, texCoord}, weight{ weight } {}

		glm::vec4 weight = glm::vec4(0.0f);

		static std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets()
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(4);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPCTcW, position)));
			offsets.push_back(std::pair(sizeof(color) / sizeof(color[0]), offsetof(VertexPCTcW, color)));
			offsets.push_back(std::pair(sizeof(texCoord) / sizeof(texCoord[0]), offsetof(VertexPCTcW, texCoord)));
			offsets.push_back(std::pair(sizeof(weight) / sizeof(weight[0]), offsetof(VertexPCTcW, weight)));
			return offsets;
		}

		static uint32_t getStride()
		{
			return sizeof(VertexPCTcW);
		}

		static int getType()
		{
			return (VertexType::Position | VertexType::TexCoord | VertexType::Color | VertexType::Weight);
		}
	};

	// Position, Color, Normal
	struct VertexPCN : VertexPC
	{
		VertexPCN() {}
		VertexPCN(glm::vec3 position, glm::vec3 color, glm::vec3 normal) :
			VertexPC(position, color), normal(normal) {}

		glm::vec3 normal = glm::vec3(0.0f);

		static std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets()
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(3);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPCN, position)));
			offsets.push_back(std::pair(sizeof(color) / sizeof(color[0]), offsetof(VertexPCN, color)));
			offsets.push_back(std::pair(sizeof(normal) / sizeof(normal[0]), offsetof(VertexPCN, normal)));
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

	// Position, Color, Normal, Weight
	struct VertexPCNW : VertexPCN
	{
		VertexPCNW() = default;
		VertexPCNW(glm::vec3 position, glm::vec3 color, glm::vec3 normal, glm::vec4 weight) :
			VertexPCN{ position, color, normal }, weight{ weight } {}

		glm::vec4 weight = glm::vec4(0.0f);

		static std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets()
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(4);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPCNW, position)));
			offsets.push_back(std::pair(sizeof(color) / sizeof(color[0]), offsetof(VertexPCNW, color)));
			offsets.push_back(std::pair(sizeof(normal) / sizeof(normal[0]), offsetof(VertexPCNW, normal)));
			offsets.push_back(std::pair(sizeof(weight) / sizeof(weight[0]), offsetof(VertexPCNW, weight)));
			return offsets;
		}

		static uint32_t getStride()
		{
			return sizeof(VertexPCNW);
		}

		static int getType()
		{
			return (VertexPCN::getType() | VertexType::Weight);
		}
	};

	// Position, Texture Coordinate, Normal
	struct VertexPTcN : VertexPTc
	{
		VertexPTcN() {}
		VertexPTcN(glm::vec3 position, glm::vec2 texCoord, glm::vec3 normal) :
			VertexPTc(position, texCoord), normal(normal) {}
		
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

	// Position, Texture Coordinate, Normal
	struct VertexPTcNW : VertexPTcN
	{
		VertexPTcNW() = default;
		VertexPTcNW(glm::vec3 position, glm::vec2 texCoord, glm::vec3 normal, glm::vec4 weight) :
			VertexPTcN{ position, texCoord, normal }, weight{ weight }
		{}

		glm::vec4 weight = glm::vec4(0.0f);

		static std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets()
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(4);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPTcNW, position)));
			offsets.push_back(std::pair(sizeof(texCoord) / sizeof(texCoord[0]), offsetof(VertexPTcNW, texCoord)));
			offsets.push_back(std::pair(sizeof(normal) / sizeof(normal[0]), offsetof(VertexPTcNW, normal)));
			offsets.push_back(std::pair(sizeof(weight) / sizeof(weight[0]), offsetof(VertexPTcNW, weight)));
			return offsets;
		}

		static uint32_t getStride()
		{
			return sizeof(VertexPTcNW);
		}

		static int getType()
		{
			return (VertexPTcN::getType() | VertexType::Weight);
		}
	};

	// Position Texture Coordinates, Normal, Tangent, Bitangent
	struct VertexPTcNTB : VertexPTcN
	{
		VertexPTcNTB() {}
		VertexPTcNTB(glm::vec3 position, glm::vec2 texCoord, glm::vec3 normal, glm::vec3 tangent, glm::vec3 bitangent) :
			VertexPTcN(position, texCoord, normal), tangent(tangent), bitangent(bitangent) {}

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

	// Position Texture Coordinates, Normal, Tangent, Bitangent, Weight
	struct VertexPTcNTBW : VertexPTcNTB
	{
		VertexPTcNTBW() {}
		VertexPTcNTBW(glm::vec3 position, glm::vec2 texCoord, glm::vec3 normal, glm::vec3 tangent, glm::vec3 bitangent, glm::vec4 weight) :
			VertexPTcNTB(position, texCoord, normal, tangent, bitangent), weight{ weight }{}

		glm::vec4 weight = glm::vec4(0.0f);

		static std::vector<std::pair<uint32_t, uint32_t>> getSizeAndOffsets()
		{
			std::vector<std::pair<uint32_t, uint32_t>> offsets;
			offsets.reserve(6);
			offsets.push_back(std::pair(sizeof(position) / sizeof(position[0]), offsetof(VertexPTcNTBW, position)));
			offsets.push_back(std::pair(sizeof(texCoord) / sizeof(texCoord[0]), offsetof(VertexPTcNTBW, texCoord)));
			offsets.push_back(std::pair(sizeof(normal) / sizeof(normal[0]), offsetof(VertexPTcNTBW, normal)));
			offsets.push_back(std::pair(sizeof(tangent) / sizeof(tangent[0]), offsetof(VertexPTcNTBW, tangent)));
			offsets.push_back(std::pair(sizeof(bitangent) / sizeof(bitangent[0]), offsetof(VertexPTcNTBW, bitangent)));
			offsets.push_back(std::pair(sizeof(weight) / sizeof(weight[0]), offsetof(VertexPTcNTBW, weight)));
			return offsets;
		}

		static uint32_t getStride()
		{
			return sizeof(VertexPTcNTBW);
		}

		static int getType()
		{
			return (VertexPTcNTB::getType() | VertexType::Weight);
		}
	};

	// Register new vertex type so compiler will know how to access to them
	using VertexRegisterTypes = Core::Utils::RegisterTypes<VertexP, VertexPW, VertexPC, VertexPCW, VertexPN, VertexPNW, VertexPTc, VertexPTcW, VertexPCTc, VertexPCTcW, VertexPCN, VertexPCNW, VertexPTcN, VertexPTcNW, VertexPTcNTB, VertexPTcNTBW>;
}
