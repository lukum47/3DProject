#include "objects/GameObject.h"
#include "render/ShaderProgram.h"
#include "render/texture2D.h"
#include "transformation/Transform.h"
#include <memory>
#include <objects/Cube.h>
#include <string>
#include <iostream>

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
    
}

void Cube::setupBuffers()
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
    glVertexAttribDivisor(3, 1); // обновляется 1 раз на инстанс

    glBindVertexArray(0);
}

void Cube::initShader()
{
    std::string vertShaderPath = VERTEX_SHADER_PATH(Cube);
    std::string fragShaderPath = FRAGMENT_SHADER_PATH(Cube);

    program = std::make_shared<ShaderProgram>(vertShaderPath, fragShaderPath);

}

void Cube::textureFromFile(std::string textureName)
{
    int width, height, nrChannels;
    std::string path = TEXTURE_DIRECTORY + textureName;
    auto *image = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
 if(image) {
                texture = std::make_shared<Texture2D>(width, height, image, nrChannels, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);
                stbi_image_free(image);
           }
    else   {
	        	std::cout << "Texture failed to load at path: " << path << '\n';
	        	stbi_image_free(image);
	       }

    
}

Cube::Cube(glm::vec3 position, std::string& textureName, BuffersPtr& buffers)
    : instanceBuffers(buffers)
{
    if (buffers->instancePositions.empty()) {
        setupBuffers();
    }
    transformer = std::make_shared<Transform>();
    transformer->translate(position);
    createCube(position);
    initShader();
    textureFromFile(textureName);
}

void Cube::draw(float deltaTime)
{
    if (!program || !transformer) return;
    
    program->use();
    
        program->setMtrix4("model", transformer->getModelMatrix());
        program->setMtrix4("view", transformer->getViewMatrix());
        program->setMtrix4("projection", transformer->getProjectionMatrix());
        glActiveTexture(GL_TEXTURE0);
        texture->bind();
        program->setInt("materials.diffuse", 0);
    
    
    // Отрисовка куба
    glBindVertexArray(instanceBuffers->VAO);
    glDrawElementsInstanced(GL_TRIANGLES, sizeof(cubeIndices)/sizeof(unsigned int), GL_UNSIGNED_INT, nullptr, instanceBuffers->instancePositions.size());
    glBindVertexArray(0);
}

void Cube::createCube(glm::vec3 position)
{
    auto& buffers = instanceBuffers;
    buffers->instancePositions.push_back(position);

    glBindBuffer(GL_ARRAY_BUFFER, buffers->instanceVBO);
    glBufferData(GL_ARRAY_BUFFER,
        buffers->instancePositions.size() * sizeof(glm::vec3),
        buffers->instancePositions.data(), GL_DYNAMIC_DRAW);
}