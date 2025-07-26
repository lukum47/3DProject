
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


void objectFabric::createModel(glm::vec3 position, const char* modelFileName, std::string nameForSave)
{
    std::shared_ptr<Model> model = std::make_shared<Model>(position, modelFileName);
    if(models.find(nameForSave) == models.end()) {
        models.emplace(nameForSave, model);
        modelList.push_back(nameForSave);
    }
}

void objectFabric::createCube(glm::vec3 position, std::string& textureName)
{
     if (objBuffers.find(SimpleObject::simpleObjectType::OBJECT_CUBE) == objBuffers.end())
    {
        // Если буферов нет - создаем новые
        instanceBuffers = std::make_shared<ObjectBuffers>();
    }
     else
    {
        // Если буферы уже есть - используем их
        instanceBuffers = objBuffers[SimpleObject::simpleObjectType::OBJECT_CUBE];
    }
        // Создаем новый куб
simpleObjects[SimpleObject::simpleObjectType::OBJECT_CUBE].emplace_back(
    std::make_unique<Cube>(position, textureName, instanceBuffers));
      objBuffers[SimpleObject::simpleObjectType::OBJECT_CUBE] = instanceBuffers;
}

void objectFabric::updateCube()
{
    auto itr = simpleObjects.find(SimpleObject::simpleObjectType::OBJECT_CUBE);
    if (itr == simpleObjects.end()) {
        return; // Нет кубов для обновления
    }

    // Получаем вектор unique_ptr<GameObject>
    auto& cubeObjects = itr->second;

    for (auto& cubePtr : cubeObjects) {
        // Получаем weak_ptr из Transform (предполагается, что getTransform() возвращает weak_ptr)
        std::weak_ptr<Transform> weakTransform = cubePtr->getTransform();
        
        // Пытаемся получить shared_ptr из weak_ptr
        if (auto sharedTransform = weakTransform.lock()) {
            // Устанавливаем матрицу вида и рисуем куб
            sharedTransform->setView(viewMatrix);
            sharedTransform->setProjection(projecctionMatrix);
            cubePtr->draw();
        }
    }
}

void objectFabric::deleteSimpleObject(SimpleObject::simpleObjectType objectType, unsigned int objectIndex)
{   
    auto simpleObjectArray = simpleObjects[objectType];
    if (objectIndex != simpleObjectArray.size() - 1)
        {
            std::swap(simpleObjectArray[objectIndex], simpleObjectArray.back());
        }
    simpleObjectArray[objectIndex]->setInstancePositionID(objectIndex);
    
}

void objectFabric::updateModels(const std::shared_ptr<Camera>& camera)
{
   this->viewMatrix = camera->getViewMatrix();
   this->projecctionMatrix = camera->getProjectionMatrix();
    for (const auto &name : modelList)
    {
         std::weak_ptr<Model> weakModel;
        try 
        {
            weakModel = models[name];
        
        if (auto sharedModel = weakModel.lock()) {
            std::weak_ptr<Transform> weakTransform = sharedModel->getTransform();

            // Пытаемся получить shared_ptr из weak_ptr
            if (auto sharedTransform = weakTransform.lock())
            {
                // Устанавливаем матрицу вида и рисуем куб
                sharedTransform->setView(viewMatrix);
                sharedTransform->setProjection(projecctionMatrix);
                sharedModel->draw();
            }
        }
        } 
        catch (const std::out_of_range& e) 
        {
            std::cerr << "Модель " << name << " не найдена: " << e.what() << "\n";
            
        }
    }
}

auto objectFabric::getModel(const std::string& modelName) -> std::weak_ptr<Model>
{
    return models.at(modelName);
}



void objectFabric::createSimpleObject(SimpleObject::simpleObjectType type, glm::vec3 position, std::string textureName)
{
     switch (type) 
    {
        case SimpleObject::simpleObjectType::OBJECT_CUBE : 
        createCube(position, textureName);
        break;
    }
}


