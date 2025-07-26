#pragma once
#include "ext/matrix_float4x4.hpp"
#include "ext/vector_float3.hpp"
#include "objects/Cube.h"
#include "resources/Model.h"
#include <memory>
#include <unordered_map>
#include <objects/GameObject.h>
#include <vector>

#pragma pack (push, 1)

 struct ObjectBuffers 
        {
        unsigned int VAO,VBO, EBO, instanceVBO;
        std::vector<glm::vec3> instancePositions;
        };
 #pragma pack (pop)

class objectFabric 
{
    private:
        std::shared_ptr<ObjectBuffers> instanceBuffers;
        std::vector<glm::vec3> instancePositions;
        glm::mat4 viewMatrix;
        glm::mat4 projecctionMatrix;
        std::pmr::unordered_map<SimpleObject::simpleObjectType,  std::shared_ptr<ObjectBuffers>> objBuffers;

        
        std::pmr::unordered_map<SimpleObject::simpleObjectType, std::vector<std::shared_ptr<SimpleObject>>> simpleObjects;
        std::pmr::unordered_map<std::string, std::shared_ptr<Model>> models;
        std::vector<std::string> modelList;
        void createCube(glm::vec3 position, std::string& textureName);
        void updateCube();
    public:
        void deleteSimpleObject(SimpleObject::simpleObjectType objectType, unsigned int objectIndex);
        void updateModels(const std::shared_ptr<Camera>& camera);
        auto getModel(const std::string& modelName) -> std::weak_ptr<Model>;
        void createSimpleObject(SimpleObject::simpleObjectType type, glm::vec3 position, std::string textureName);
        void updateSimpleObjects(const std::shared_ptr<Camera>& camera);
        void createModel(glm::vec3 position, const char* modelFileName, std::string nameForSave);
};