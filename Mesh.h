#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include"Shader.h"
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>
#include <vector>
#include"PipelineManager.h"
#include"MeshProperty.h"
#include"_3DBlueNoiseSampler.h"
#include<qdebug.h>

class Mesh {
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    PoissonSampler sampler;
    unsigned int VAO;

    int td = -1;
    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();

        sampler.sample(vertices, indices, 0.5f);
        vector<vec2> t = sampler.get_texture();
        int tl = 256;
        vector<vector<vec4>> tex(tl, vector<vec4>(tl, vec4(0, 0, 0,255)));
        for (int i = 0; i < t.size(); i++) {
            int x_i = floor(t[i].x * (double)tl);
            int y_i = floor(t[i].y * (double)tl);
            if (x_i < 0) {
                x_i = 0;
            }
            if (x_i > tl - 1) {
                x_i = tl - 1;
            }

            if (y_i < 0) {
                y_i = 0;
            }
            if (y_i > tl - 1) {
                y_i = tl - 1;
            }
            tex[x_i][y_i] = vec4(255, 255, 255, 255);
        }
        td = PipelineManager::bind4Map(tex);
    }
    // render the mesh
    void Draw(Shader& shader)
    {
        QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            f->glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to string
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to string
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
            int loc = f->glGetUniformLocation(shader.ID, ("material_" + name + "0").c_str());
            f->glUniform1i(loc, i);
            // and finally bind the texture
            f->glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        
        //int generated_map = PipelineManager::shared->output->diffuse_map;
        int generated_map = td;
        if (generated_map > 0) {
            f->glActiveTexture(GL_TEXTURE0);
            shader.setInt("material_texture_diffuse0", 0);
            f->glBindTexture(GL_TEXTURE_2D, generated_map);
        }

        // draw mesh
        f->glBindVertexArray(VAO);
        f->glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        f->glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        f->glActiveTexture(GL_TEXTURE0);
    }
    void setupMesh()
    {
        QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
        // create buffers/arrays
        f->glGenVertexArrays(1, &VAO);
        f->glGenBuffers(1, &VBO);
        f->glGenBuffers(1, &EBO);

        f->glBindVertexArray(VAO);
        // load data into vertex buffers
        f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        f->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        f->glEnableVertexAttribArray(0);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        f->glEnableVertexAttribArray(1);
        f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        f->glEnableVertexAttribArray(2);
        f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // vertex tangent
        f->glEnableVertexAttribArray(3);
        f->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        f->glEnableVertexAttribArray(4);
        f->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
        // ids
        f->glEnableVertexAttribArray(5);
        f->glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        // weights
        f->glEnableVertexAttribArray(6);
        f->glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
        f->glBindVertexArray(0);
    }
    void release_mesh() {
        QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
        f->glDeleteVertexArrays(1, &VAO);
        f->glDeleteBuffers(1, &VBO);
        f->glDeleteBuffers(1, &EBO);
    }
    ~Mesh() {
        //release_mesh();
    }
private:
    // render data 
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays

};
