#pragma once

#include <memory>
#include <render/ShaderProgram.h>
#include <resources/Model.h>
#include <resources/camera.h>
#include <render/texture2D.h>
#include <transformation/Transform.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <string>

#ifdef _WIN32
#define VERTEX_SHADER_PATH(className) "D:/cpp/1/openProject/res/shader/" #className ".vert"
#define FRAGMENT_SHADER_PATH(className) "D:/cpp/1/openProject/res/shader/" #className ".frag"
#define TEXTURE_DIRECTORY "D:/cpp/1/openProject/res/textures/"

#else
#define VERTEX_SHADER_PATH(className) "/Users/bogdan/cpp/openProject/res/shader/" #className ".vert"
#define FRAGMENT_SHADER_PATH(className) "/Users/bogdan/cpp/openProject/res/shader/" #className ".frag"
#define TEXTURE_DIRECTORY "/Users/bogdan/cpp/openProject/res/textures/" 

#endif // _WIN32


using namespace Render;
using namespace glm;
using namespace Geometry;



class GameObject {
protected: 
    std::shared_ptr<Transform> transformer;
    std::shared_ptr<ShaderProgram> program;
    std::shared_ptr<Texture2D> texture;
    
    virtual void setupBuffers() = 0;
    virtual void initShader() = 0;
    virtual void textureFromFile(std::string texturePath) = 0;
public:
        enum class simpleObjectType : std::uint8_t 
        {
            OBJECT_CUBE
        };

    virtual ~GameObject() = default;
    virtual void draw(float deltaTime) = 0;
    virtual std::shared_ptr<ShaderProgram> getShader() = 0;
    virtual std::weak_ptr<Transform> getTransform() = 0;
    virtual void switchLighting() = 0;
       
};