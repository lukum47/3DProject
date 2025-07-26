#include "objects/GameObject.h"
#include "render/ShaderProgram.h"
#include "render/texture2D.h"
#include "transformation/Transform.h"
#include <memory>
#include <objects/Cube.h>
#include <string>
#include <iostream>
#include "Cube.h"

std::weak_ptr<Transform> Cube::getTransform()

{
    return transformer;
}

std::shared_ptr<ShaderProgram> Cube::getShader()
{
    return program;
}

void Cube::switchLighting()
{
    // Implementation goes here
}

void Cube::loadObject()
{
    auto& buffers = instanceBuffers;

    glGenVertexArrays(1, &buffers->VAO);
    glBindVertexArray(buffers->VAO);

    glGenBuffers(1, &buffers->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, buffers->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &buffers->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // instanced positions
    glGenBuffers(1, &buffers->instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, buffers->instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);
}

void Cube::initShader()
{
    std::string vertShaderPath = VERTEX_SHADER_PATH(Cube);
    std::string fragShaderPath = FRAGMENT_SHADER_PATH(Cube);
    program = std::make_shared<ShaderProgram>(vertShaderPath, fragShaderPath);
}

Cube::Cube(glm::vec3 position, std::string& textureName, BuffersPtr& buffers)
{
    instanceBuffers = {buffers};
    if (buffers->instancePositions.empty()) {
        Cube::loadObject();
    }
    
    transformer = std::make_shared<Transform>();
    transformer->translate(position);
    createCube(position);
    Cube::initShader();
    
    try {
        texture = textureFromFile(textureName);
    }
    catch (const std::exception& e) {
        std::cerr << "Cube construction failed: " << e.what() << '\n';
        throw;
    }
}

Cube::~Cube()
{
          if (!instanceBuffers->instancePositions.empty() && instancePositionID < instanceBuffers->instancePositions.size())
    {
         
                if (instancePositionID != instanceBuffers->instancePositions.size() - 1)
                    {
                          std::swap(instanceBuffers->instancePositions[instancePositionID], 
                          instanceBuffers->instancePositions.back());

                    }
                instanceBuffers->instancePositions.pop_back();
                    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffers->instanceVBO);
        glBufferData(GL_ARRAY_BUFFER,
                    instanceBuffers->instancePositions.size() * sizeof(glm::vec3),
                    instanceBuffers->instancePositions.data(), GL_DYNAMIC_DRAW);
    }
}


void Cube::draw()
{
    if (!program || !transformer) return;
    
    program->use();
    program->setMtrix4("model", transformer->getModelMatrix());
    program->setMtrix4("view", transformer->getViewMatrix());
    program->setMtrix4("projection", transformer->getProjectionMatrix());
    
    glActiveTexture(GL_TEXTURE0);
    texture->bind();
    program->setInt("materials.diffuse", 0);
    
    glBindVertexArray(instanceBuffers->VAO);
    glDrawElementsInstanced(GL_TRIANGLES, sizeof(cubeIndices)/sizeof(unsigned int), 
                          GL_UNSIGNED_INT, nullptr, instanceBuffers->instancePositions.size());
    glBindVertexArray(0);
}

void Cube::createCube(glm::vec3 position)
{
    auto& buffers = instanceBuffers;
    buffers->instancePositions.push_back(position);
    instancePositionID = buffers->instancePositions.size() - 1;
    glBindBuffer(GL_ARRAY_BUFFER, buffers->instanceVBO);
    glBufferData(GL_ARRAY_BUFFER,
                buffers->instancePositions.size() * sizeof(glm::vec3),
                buffers->instancePositions.data(), GL_DYNAMIC_DRAW);
}