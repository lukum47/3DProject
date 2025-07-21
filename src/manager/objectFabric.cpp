
#include "ext/vector_float3.hpp"
#include "objects/Cube.h"
#include "render/ShaderProgram.h"
#include <algorithm>
#include <iterator>
#include <manager/objectFabric.h>
#include <memory>
#include <utility>
#include <vector>



void objectFabric::updateSimpleObjects(const std::shared_ptr<Camera>& camera)
{
   this->viewMatrix = camera->getViewMatrix();
   this->projecctionMatrix = camera->getProjectionMatrix();
   updateCube();
}


void objectFabric::createModel(std::string& pathToModel)
{
    
}

void objectFabric::createCube(glm::vec3 position, std::string& textureName)
{
     if (objBuffers.find(GameObject::simpleObjectType::OBJECT_CUBE) == objBuffers.end())
    {
        // Если буферов нет - создаем новые
        instanceBuffers = std::make_shared<ObjectBuffers>();
    }
    else
    {
        // Если буферы уже есть - используем их
        instanceBuffers = objBuffers[GameObject::simpleObjectType::OBJECT_CUBE];
    }
        // Создаем новый куб
simpleObjects[GameObject::simpleObjectType::OBJECT_CUBE].emplace_back(
    std::make_unique<Cube>(position, textureName, instanceBuffers));
      objBuffers[GameObject::simpleObjectType::OBJECT_CUBE] = instanceBuffers;
}

void objectFabric::updateCube()
{
    auto it = simpleObjects.find(GameObject::simpleObjectType::OBJECT_CUBE);
    if (it == simpleObjects.end()) {
        return; // Нет кубов для обновления
    }

    // Получаем вектор unique_ptr<GameObject>
    auto& cubeObjects = it->second;

    for (auto& cubePtr : cubeObjects) {
        // Получаем weak_ptr из Transform (предполагается, что getTransform() возвращает weak_ptr)
        std::weak_ptr<Transform> weakTransform = cubePtr->getTransform();
        
        // Пытаемся получить shared_ptr из weak_ptr
        if (auto sharedTransform = weakTransform.lock()) {
            // Устанавливаем матрицу вида и рисуем куб
            sharedTransform->setView(viewMatrix);
            sharedTransform->setProjection(projecctionMatrix);
            cubePtr->draw(deltaTime);
        }
        // Если weak_ptr невалиден, объект уже удален, пропускаем
    }
}

void objectFabric::setDeltaTime(float deltaTime)
{
      this->deltaTime = deltaTime;
}

void objectFabric::createSimpleObject(GameObject::simpleObjectType type, glm::vec3 position, std::string textureName)
{
     switch (type) 
    {
        case GameObject::simpleObjectType::OBJECT_CUBE : 
        createCube(position, textureName);
        break;
    }
}


