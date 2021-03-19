#pragma once

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <filesystem>

namespace Graphics
{
    class Model
    {
    public:
        Model(const std::filesystem::path& path)
        {
            loadModel(path.string());
        }
        void Draw(ShaderProgram& shader) const;
    private:
        // model data
        std::vector<Mesh> m_Meshes;
        std::filesystem::path m_Directory;
        std::vector<Texture> m_LoadedTextures;

        void loadModel(const std::filesystem::path& path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
            std::string_view typeName);
    };
}