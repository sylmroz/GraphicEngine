#pragma once

#include "../../Common/ModelImporter.hpp"
#include "../../Common/Vertex.hpp"

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
		std::tuple<std::vector<std::shared_ptr<Scene::Mesh<Vertex>>>, std::string> read(const std::string& path)
		{
			Assimp::Importer importer;

			uint32_t flags = aiProcess_Triangulate | aiProcess_FlipUVs;
			if (Vertex::getType() & Common::VertexType::Tangent)
			{
				flags |= aiProcess_CalcTangentSpace;
			}

			auto scene = importer.ReadFile(path, flags);

			std::vector<std::shared_ptr<Scene::Mesh<Vertex>>> meshes;

			processNode(scene->mRootNode, scene, meshes);

			return std::make_tuple(std::move(meshes), std::string());
		}

	private:
		void processNode(aiNode* node, const aiScene* scene, std::vector<std::shared_ptr<Scene::Mesh<Vertex>>>& meshes)
		{
			for (uint32_t i{ 0 }; i < node->mNumMeshes; ++i)
			{
				meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]]));
			}

			for (uint32_t i{ 0 }; i < node->mNumChildren; ++i)
			{
				processNode(node->mChildren[i], scene, meshes);
			}
		}

		std::shared_ptr<Scene::Mesh<Vertex>> processMesh(aiMesh* mesh)
		{
			std::shared_ptr<Scene::Mesh<Vertex>> outMesh = std::make_shared<Scene::Mesh<Vertex>>();

			outMesh->resizeVertices(mesh->mNumVertices);

			uint32_t numIndices{ 0 };
			for (uint32_t i{ 0 }; i < mesh->mNumFaces; ++i)
			{
				numIndices += mesh->mFaces[i].mNumIndices;
			}

			outMesh->resizeIndices(numIndices);

			for (uint32_t i{ 0 }; i < mesh->mNumFaces; ++i)
			{
				for (uint32_t j{ 0 }; j < mesh->mFaces[i].mNumIndices; ++j)
				{
					outMesh->addIndex(mesh->mFaces[i].mIndices[j]);
				}
			}

			for (uint32_t i{ 0 }; i < mesh->mNumVertices; ++i)
			{
				Vertex vertex;

				if (mesh->HasPositions())
				{
					glm::vec3 position;
					position.x = mesh->mVertices[i].x;
					position.y = mesh->mVertices[i].y;
					position.z = mesh->mVertices[i].z;
					vertex.position = position;
				}

				if (mesh->HasNormals())
				{
					if constexpr (std::is_same_v<Vertex, Common::VertexPN> &&
						std::is_same_v<Vertex, Common::VertexPTcN> &&
						std::is_same_v<Vertex, Common::VertexPTcNTB>)
					{
						glm::vec3 normal;
						normal.x = mesh->mNormals[i].x;
						normal.y = mesh->mNormals[i].y;
						normal.z = mesh->mNormals[i].z;
						vertex.normal = normal;
					}
				}

				if (mesh->HasVertexColors(0))
				{
					if constexpr (std::is_same_v<Vertex, Common::VertexPC> &&
						std::is_same_v<Vertex, Common::VertexPCTc>)
					{
						glm::vec3 color;
						color.x = mesh->mColors[i]->r;
						color.y = mesh->mColors[i]->g;
						color.z = mesh->mColors[i]->b;
						vertex.color = color;
					}
				}

				if (mesh->HasTextureCoords(0) && mesh->mTextureCoords[0])
				{
					if constexpr (std::is_same_v<Vertex, Common::VertexPTc> &&
						std::is_same_v<Vertex, Common::VertexPCTc> && 
						std::is_same_v<Vertex, Common::VertexPTcNTB>)
					{
						glm::vec2 color;
						color.x = mesh->mTextureCoords[0][i].x;
						color.y = mesh->mTextureCoords[0][i].y;
						vertex.texCoord = color;
					}
				}

				if (mesh->HasTangentsAndBitangents())
				{
					if constexpr (std::is_same_v<Vertex, Common::VertexPTcNTB>)
					{
						glm::vec3 tangent;
						tangent.x = mesh->mNormals[i].x;
						tangent.y = mesh->mNormals[i].y;
						tangent.z = mesh->mNormals[i].z;
						vertex.tangent = tangent;
					}
				}

				outMesh->addVertex(vertex);
			}

			if (mesh->HasBones() && Vertex::getType() & Common::VertexType::Bone)
			{
				// TODO process bones
			}

			return outMesh;
		}
	};
}