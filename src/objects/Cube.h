#pragma once 
#include "ext/vector_float3.hpp"
#include "render/ShaderProgram.h"
#include <memory>
#include <objects/GameObject.h>
#include <manager/objectFabric.h>
#ifdef _WIN32
#include <windows.h>  // Windows-версия потоков
#else
#include <pthread.h>  // POSIX-версия
#endif
#include <stdexcept>
#include <vector>

struct ObjectBuffers;

using BuffersPtr = std::shared_ptr<ObjectBuffers>;
using BuffersRef = std::weak_ptr<ObjectBuffers>;

class Cube : public GameObject {
private:
    BuffersPtr instanceBuffers;

    static constexpr float cubeVertices[] = {
        // Позиции           // Нормали          // Текстурные координаты
        // Передняя грань
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
        
        // Задняя грань
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        
        // Левая грань
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        
        // Правая грань
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        
        // Нижняя грань
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        
        // Верхняя грань
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f
    };

    static constexpr unsigned int cubeIndices[] = {
        // Передняя грань
        0, 1, 2,
        2, 3, 0,
        
        // Задняя грань
        4, 5, 6,
        6, 7, 4,
        
        // Левая грань
        8, 9, 10,
        10, 11, 8,
        
        // Правая грань
        12, 13, 14,
        14, 15, 12,
        
        // Нижняя грань
        16, 17, 18,
        18, 19, 16,
        
        // Верхняя грань
        20, 21, 22,
        22, 23, 20
    };

    void createCube(glm::vec3 position);
    void setupBuffers() override;
    void initShader() override;
    void textureFromFile(std::string textureName) override;
public: 
    Cube(glm::vec3 position, std::string& textureName, BuffersPtr& buffers);
    ~Cube() override = default;
    void draw(float deltaTime) override;
    std::weak_ptr<Transform> getTransform() override;
    std::shared_ptr<ShaderProgram> getShader() override;
    void switchLighting() override;
};