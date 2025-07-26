#pragma once
#include "../render/ShaderProgram.h"
#include <assimp/types.h>
#include <fstream>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;


#pragma pack (push, 1)
struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
 };

struct Texture {
    std::shared_ptr<Texture2D> texture;
    string type;
    string path;
};
#pragma pack (pop)
class Mesh {
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    unsigned int VAO = 0;

    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
        : vertices(std::move(vertices)), indices(std::move(indices)), textures(std::move(textures))
    {
        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void Draw(const std::shared_ptr<Render::ShaderProgram>& shader)
    {
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if (name == "materials.diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "materials.specular")
                number = std::to_string(specularNr++); // transfer unsigned int to string
            else if (name == "materials.normal")
                number = std::to_string(normalNr++); // transfer unsigned int to string
            else if (name == "materials.height")
                number = std::to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
            shader->setInt((name), i);
            // and finally bind the texture
            textures[i].texture->bind();
        }

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
       glActiveTexture(GL_TEXTURE0);
    }

private:
    // render data 
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    // initializes all the buffer objects/arrays
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
   
    }
}; 