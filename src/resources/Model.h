#pragma once 
//#include "Windows.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <objects/GameObject.h>
#include <iostream>
#include <string>
#include <vector>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../render/ShaderProgram.h"
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <memory>


//#define MAX_BONE_INFLUENCE 4
#pragma pack(push, 1)
struct Header {
	uint16_t signature;
	uint16_t numOfMeshes;
};

struct MyTextures {
	std::string type;
	std::string fileName;
};

#pragma pack(pop)

struct FileMapping {

	int hFile; 
	size_t fsize;  
	unsigned char* dataPtr; 
};
struct validTextures {
	bool diffuse;
	bool specular;
};
class Model : public GameObject{
public:
	explicit Model(glm::vec3 position, const std::string& modelFileName) {
	initTransform(position);
	modelPath = MODELS_DIRECTORY + modelFileName;
	Model::loadObject();
	Model::initShader();
    }
//	void Draw(Render::ShaderProgram &program);
	void draw() override;
	void switchLighting() override;
	~Model() override = default;
private:
	std::string modelPath;

	unsigned int byteCount = 0;
	std::vector<Mesh> meshes;
	std::vector<Texture> loadedTextures;
	void loadObject() override;
	void loadModel(const std::string& path);
	 void processNode(aiNode* node, const aiScene* scene);
	 auto processMesh(std::unique_ptr<FileMapping>& fMapping, const int& vertSize, const int& indxSize, const int& texSize) -> Mesh;
	 void loadConvertedModel(const std::string& path);
	auto processMesh(aiMesh* mesh, const aiScene* scene) -> Mesh;
	auto loadMaterialTexture( aiMaterial* mat, aiTextureType type, const char* typeName) -> std::vector<Texture>;
    void initShader() override;
    
    
};