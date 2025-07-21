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
        float deltaTime = 0;
        std::vector<glm::vec3> instancePositions;
        glm::mat4 viewMatrix;
        glm::mat4 projecctionMatrix;
        std::pmr::unordered_map<GameObject::simpleObjectType,  std::shared_ptr<ObjectBuffers>> objBuffers;
        std::pmr::unordered_map<GameObject::simpleObjectType, std::vector<std::unique_ptr<GameObject>>> simpleObjects;
        std::pmr::unordered_map<std::string, Model> models;
        void createCube(glm::vec3 position, std::string& textureName);
        void updateCube();
    public:
        void setDeltaTime(float deltaTime);
        void createSimpleObject(GameObject::simpleObjectType type, glm::vec3 position, std::string textureName);
        void updateSimpleObjects(const std::shared_ptr<Camera>& camera);
        void createModel(std::string& pathToModel);
};