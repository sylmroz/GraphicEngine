#pragma once

#include "../../Common/ModelImporter.hpp"
#include "../../Common/Vertex.hpp"
#include "../../Core/Utils/MemberTraits.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

namespace GraphicEngine::Modules
{
	template <typename Vertex>
	class AssimpModelImporter : public Common::ModelImporter<AssimpModelImporter<Vertex>, Vertex>
	{
	public:
		std::vector<std::shared_ptr<Scene::Model<Vertex>>> read(const std::string& path)
		{
			Assimp::Importer importer;

			uint32_t flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_FindInvalidData | aiProcess_FindDegenerates;
			if (Vertex::getType() & Common::VertexType::Normal)
			{
				flags |= (aiProcess_GenNormals | aiProcess_FixInfacingNormals);
			}
			if (Vertex::getType() & Common::VertexType::Tangent)
			{
				flags |= aiProcess_CalcTangentSpace;
			}

			auto scene = importer.ReadFile(path, flags);

			std::vector<std::shared_ptr<Scene::Model<Vertex>>> models;

			processNode(scene->mRootNode, scene, models, -1);

			return std::move(models);
		}

	private:
		void processNode(aiNode* node, const aiScene* scene, std::vector<std::shared_ptr<Scene::Model<Vertex>>>& models, int32_t parentId)
		{
			std::vector<std::shared_ptr<Scene::Mesh<Vertex>>> meshes;
			meshes.reserve(node->mNumMeshes);
			for (uint32_t i{ 0 }; i < node->mNumMeshes; ++i)
			{
				meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]]));
			}

			if (meshes.size() > 0)
			{
				models.push_back(std::make_shared<Scene::Model<Vertex>>(meshes, node->mName.C_Str()));
				models.back()->setId(models.size() - 1);
				if (parentId > -1)
				{
					models[parentId]->addChildrenId(models.back()->getId());
				}
			}

			for (uint32_t i{ 0 }; i < node->mNumChildren; ++i)
			{
				processNode(node->mChildren[i], scene, models, models.size() > 0 ? models.back()->getId() : parentId);
			}
		}

		std::shared_ptr<Scene::Mesh<Vertex>> processMesh(aiMesh* mesh)
		{
			std::shared_ptr<Scene::Mesh<Vertex>> outMesh = std::make_shared<Scene::Mesh<Vertex>>();

			outMesh->resizeVertices(mesh->mNumVertices);

			int generateResources = Common::VertexType::Position;
			for (uint32_t i{ 0 }; i < mesh->mNumVertices; ++i)
			{
				auto vertex = std::make_shared<Vertex>();

				if (mesh->HasPositions())
				{
					glm::vec3 position;
					position.x = mesh->mVertices[i].x;
					position.y = mesh->mVertices[i].y;
					position.z = mesh->mVertices[i].z;
					vertex->position = position;
				}

				if constexpr (Core::Utils::has_normal_member<Vertex>::value)
				{
					if (mesh->HasNormals())
					{
						glm::vec3 normal;
						normal.x = mesh->mNormals[i].x;
						normal.y = mesh->mNormals[i].y;
						normal.z = mesh->mNormals[i].z;
						vertex->normal = normal;
					}

					else
					{
						generateResources = generateResources | Common::VertexType::Normal;
					}
				}

				if constexpr (Core::Utils::has_color_member<Vertex>::value)
				{
					if (mesh->HasVertexColors(0))
					{
						glm::vec3 color;
						color.x = mesh->mColors[i]->r;
						color.y = mesh->mColors[i]->g;
						color.z = mesh->mColors[i]->b;
						vertex->color = color;
					}
				}

				if constexpr (Core::Utils::has_texCoord_member<Vertex>::value)
				{
					if (mesh->HasTextureCoords(0) && mesh->mTextureCoords[0])
					{
						glm::vec2 texCoord;
						texCoord.x = mesh->mTextureCoords[0][i].x;
						texCoord.y = mesh->mTextureCoords[0][i].y;
						vertex->texCoord = texCoord;
					}
				}

				if constexpr (Core::Utils::has_tangent_member<Vertex>::value && Core::Utils::has_bitangent_member<Vertex>::value)
				{
					if (mesh->HasTangentsAndBitangents())
					{
						glm::vec3 tangent;
						tangent.x = mesh->mTangents[i].x;
						tangent.y = mesh->mTangents[i].y;
						tangent.z = mesh->mTangents[i].z;
						vertex->tangent = tangent;

						glm::vec3 bitangent;
						bitangent.x = mesh->mBitangents[i].x;
						bitangent.y = mesh->mBitangents[i].y;
						bitangent.z = mesh->mBitangents[i].z;
						vertex->bitangent = bitangent;
					}

					else
					{
						generateResources = generateResources | Common::VertexType::Tangent;
					}
				}

				outMesh->addVertex(vertex);
			}

			outMesh->resizeFaces(mesh->mNumFaces);

			for (uint32_t i{ 0 }; i < mesh->mNumFaces; ++i)
			{
				auto face = std::make_shared<Scene::Face>();
				face->indices.reserve(mesh->mFaces[i].mNumIndices);
				for (uint32_t j{ 0 }; j < mesh->mFaces[i].mNumIndices; ++j)
				{
					face->indices.push_back(mesh->mFaces[i].mIndices[j]);
				}
				outMesh->addFace(face);
			}

			outMesh->generate(static_cast<Common::VertexType>(generateResources));

			return outMesh;
		}
	};
}