#include "Model.h"

void Model::draw()
{
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(program);
    }
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer import;
    
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if ((scene == nullptr) || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // Process all meshes in the node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    
    // Process all child nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(std::unique_ptr<FileMapping>& fMapping, const int& vertSize, const int& indxSize, const int& texSize)
{
    Texture textureMesh;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    
    for (int vertIt = 0; vertIt < vertSize; ++vertIt) {
        Vertex vert;
        std::memcpy(&vert, fMapping->dataPtr, sizeof(vert));
        fMapping->dataPtr += sizeof(vert);
        vertices.push_back(vert);
    }
    
    for (int indxIt = 0; indxIt < indxSize; ++indxIt) {
        uint32_t indx;
        std::memcpy(&indx, fMapping->dataPtr, sizeof(indx));
        fMapping->dataPtr += sizeof(indx);
        indices.push_back(indx);
    }
    
    MyTextures texes;
    for (int texIt = 0; texIt < texSize; ++texIt) {
        char str[100];
        std::memcpy(&str, fMapping->dataPtr, 100);
        texes.type = str;
        fMapping->dataPtr += 100;
    
        std::memcpy(&str, fMapping->dataPtr, 100);
        texes.fileName = str;
        fMapping->dataPtr += 100;
    
        bool skip = false;
        for (unsigned int j = 0; j < loadedTextures.size(); j++) {
            if (std::strcmp(loadedTextures[j].path.data(), texes.fileName.c_str()) == 0) {
                textures.push_back(loadedTextures[j]);
                skip = true;
                break;
            }
        }
        
        if (!skip) {
            textureMesh.type = texes.type;
            textureMesh.path = texes.fileName;
            textureMesh.texture = textureFromFile(textureMesh.path);
            textures.push_back(textureMesh);
            loadedTextures.push_back(textureMesh);
        }
    }
    return Mesh(vertices, indices, textures);
}

void Model::initShader() {
    std::string vertShaderPath = VERTEX_SHADER_PATH(Model);
    std::string fragShaderPath = FRAGMENT_SHADER_PATH(Model);
    program = std::make_shared<ShaderProgram>(vertShaderPath, fragShaderPath);
}

void Model::loadConvertedModel(const std::string& path)
{
    int descrypt = open(path.c_str(), O_RDONLY, 0);
    if (descrypt < 0) {
        std::cerr << "READING FAILED :: " << strerror(errno) << '\n';
        close(descrypt);
        return;
    }
    
    struct stat st;
    if(fstat(descrypt, &st) < 0) {
        std::cerr << "fileMappingCreate - fstat failed, fname = "
                << path << ", " << strerror(errno) << '\n';
        close(descrypt);
        return;
    }
    
    auto fSize = (size_t)st.st_size;
    auto* dataPtr = (unsigned char*)mmap(nullptr, fSize, PROT_READ, MAP_PRIVATE, descrypt, 0);
    if (dataPtr == MAP_FAILED) {
        std::cerr << "fileMappingCreate - CreateFile failed, fname = "
                << descrypt << strerror(errno) << '\n';
        close(descrypt);
        return;
    }
    
    std::unique_ptr<FileMapping> mapping(new FileMapping);
    if (mapping == nullptr) {
        std::cerr << "fileMappingCreate - malloc failed, fname = " << descrypt << '\n';
        munmap(dataPtr, fSize);
        close(descrypt);
        return;
    }
    
    mapping->dataPtr = dataPtr;
    mapping->fsize = fSize;
    mapping->hFile = descrypt;

    uint32_t numVert;
    uint32_t numIndx;
    uint32_t numTex;
    Header head;
    
    std::memcpy(&head, mapping->dataPtr, sizeof(head));
    mapping->dataPtr += sizeof(head);

    for (int i = 0; i < head.numOfMeshes; ++i) {
        std::memcpy(&numVert, mapping->dataPtr, sizeof(uint32_t));
        mapping->dataPtr += sizeof(uint32_t);
        std::memcpy(&numIndx, mapping->dataPtr, sizeof(uint32_t));
        mapping->dataPtr += sizeof(uint32_t);
        std::memcpy(&numTex, mapping->dataPtr, sizeof(uint32_t));
        mapping->dataPtr += sizeof(uint32_t);
        meshes.push_back(processMesh(mapping, numVert, numIndx, numTex));
    }
    
    munmap(mapping->dataPtr, mapping->fsize);
    close(mapping->hFile);
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;
        
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        
        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        
        if (mesh->mTextureCoords[0] != nullptr) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else {
            vertex.TexCoords = glm::vec2(0.F, 0.F);
        }
        
        vertices.push_back(vertex);
    }
    
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        
        std::vector<Texture> diffuseMaps = loadMaterialTexture(material, 
                                            aiTextureType_DIFFUSE, "materials.diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        
        std::vector<Texture> specularMaps = loadMaterialTexture(material,
                                            aiTextureType_SPECULAR, "materials.specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        
        std::vector<Texture> normalMaps = loadMaterialTexture(material, 
                                            aiTextureType_NORMALS, "materials.normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        
        std::vector<Texture> heightMaps = loadMaterialTexture(material, 
                                            aiTextureType_HEIGHT, "materials.height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }
    
    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTexture(aiMaterial* mat, aiTextureType type, const char* typeName)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string fName = str.C_Str();
    
        size_t fIndx = fName.find_last_of('\\');
        if (fIndx != std::string::npos) {
            fName = fName.substr(fIndx+2, fName.size());
            if (fName.substr(fName.find_last_of('.'), fName.size()) == ".tif") {
                fName = fName.substr(0, fName.find_last_of('.')) + ".png";
            }
        }
        
        bool skip = false;
        for (auto & loadedTexture : loadedTextures) {
            if (std::strcmp(loadedTexture.path.data(), fName.c_str()) == 0) {
                textures.push_back(loadedTexture);
                skip = true;
                break;
            }
        }
        
        if (!skip) {
            Texture texture;
            try {
                texture.texture = textureFromFile(fName);
                texture.type = typeName;
                texture.path = fName;
                textures.push_back(texture);
                loadedTextures.push_back(texture);
            }
            catch (const std::exception& e) {
                std::cerr << "Texture loading failed: " << e.what() << '\n';
                continue;
            }
        }
    }

    return textures;
}

void Model::loadObject() 
{
    if (modelPath.substr(modelPath.find_last_of('.'), modelPath.size()) == ".dwu") {
        loadConvertedModel(modelPath);
    }
    else {
        loadModel(modelPath);
    }
}

void Model::switchLighting() {
    // Implementation goes here
}