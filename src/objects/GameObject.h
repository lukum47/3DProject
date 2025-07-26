#pragma once


#include <memory>
#include <render/ShaderProgram.h>
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
#define MODELS_DIRECTORY "D:/cpp/1/openProject/res/model/"
#else
#define VERTEX_SHADER_PATH(className) "/Users/bogdan/cpp/3DProject/res/shader/" #className ".vert"
#define FRAGMENT_SHADER_PATH(className) "/Users/bogdan/cpp/3DProject/res/shader/" #className ".frag"
#define TEXTURE_DIRECTORY "/Users/bogdan/cpp/3DProject/res/textures/" 
#define MODELS_DIRECTORY "/Users/bogdan/cpp/3DProject/res/model/"
#endif // _WIN32

#include <stb_image.h>

using namespace Render;
using namespace glm;
using namespace Geometry;



class GameObject {
protected: 
    std::shared_ptr<Transform> transformer;
    std::shared_ptr<ShaderProgram> program;
    std::shared_ptr<Texture2D> texture;

    virtual void loadObject() = 0;
    virtual void initShader() = 0;
    
    auto textureFromFile(std::string textureName) -> std::shared_ptr<Texture2D> 
    {
     int width;
    int height;
    int nrChannels;
    std::string path = TEXTURE_DIRECTORY + textureName;
    auto *image = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	std::shared_ptr <Texture2D> tex;
    
    if(image != nullptr) 
           {
                tex = std::make_shared<Texture2D>(width, height, image, nrChannels, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);
                stbi_image_free(image);
           }
    else   {
	        	std::cout << "Texture failed to load at path: " << path << '\n';
	        	stbi_image_free(image);
	       }
    return tex;
    }

    virtual void initTransform(glm::vec3 startPosition) {
        transformer = std::make_shared<Transform>();
        transformer->translate(startPosition);
    }
public:
        

    virtual ~GameObject() = default;
    virtual void draw() = 0;
    virtual auto getShader() -> std::shared_ptr<ShaderProgram> {
        return program;
    };
    virtual auto getTransform() -> std::weak_ptr<Transform> {
        return transformer;
    };
    
    virtual void switchLighting() = 0;
       
};