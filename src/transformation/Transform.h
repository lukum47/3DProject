#pragma once 
#include "ext/matrix_float4x4.hpp"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <resources/camera.h>

namespace Geometry {
  class Transform {

    private:
        glm::vec3 position = glm::vec3(1.F);
        float rotation = 0;
        glm::vec3 curAxisDir = glm::vec3(1.F);
        glm::vec3 curScale = glm::vec3(1.F);
        glm::mat4 modelMatrix = glm::mat4(1.F);
        glm::mat4 projectionMatrix = glm::mat4(1.F);
        glm::mat4 viewMatrix = glm::mat4(1.F);
    public:

    glm::vec3 getPosition();
    void scale(float rate) {
        curScale = glm::vec3(rate, rate, rate);
        updateModelMatrix();
    }
    void translate(glm::vec3 position) {
        this->position = position;
        updateModelMatrix();
    }
    void setView(glm::mat4 viewMatrix) 
    {
        this->viewMatrix = viewMatrix;
    }
    void setProjection(glm::mat4 matrix);
    void updateModelMatrix() {
        modelMatrix = glm::mat4(1.F);
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::scale(modelMatrix, curScale);
        modelMatrix = glm::rotate(modelMatrix, rotation, curAxisDir);
    }
    void rotate(float angle, glm::vec3 axisDirection) {
        curAxisDir = axisDirection;
        rotation = angle;
        updateModelMatrix();
    }
    glm::mat4& getModelMatrix();
    glm::mat4& getViewMatrix();
    glm::mat4& getProjectionMatrix();
  
}; 
} 

