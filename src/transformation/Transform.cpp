#include "Transform.h"


glm::vec3 Geometry::Transform::getPosition()
{
    return position;
}

void Geometry::Transform::setProjection(glm::mat4 matrix)
{
    projectionMatrix = matrix;
}


glm::mat4& Geometry::Transform::getModelMatrix()
{
    return modelMatrix;
}

glm::mat4& Geometry::Transform::getProjectionMatrix()
{
    return projectionMatrix;
}


namespace Geometry
{
    glm::mat4& Transform::getViewMatrix()
    {
        return viewMatrix;
    }
}