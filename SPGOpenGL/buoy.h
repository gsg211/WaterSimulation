#pragma once
#include "Entity.h"
#include <vector>
#include <string>
#include "objloader.hpp"
#include <direct.h>

class Buoy : public Entity {
private:
    GLuint textureID;
    glm::vec3 lightPos;
    glm::mat4 modelMatrix = glm::mat4(1.0f); 

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> verticesNormals;

public:

    Buoy(glm::vec3 lp) : Entity("BuoyVertex.vert", "BuoyFragment.frag"), lightPos(lp) {
    }

    void init() override {
        load_shaders();

        if (!loadOBJ("obj/buoy.obj", vertices, uvs, normals)) {
            printf("Failed to load OBJ!\n");
        }

        verticesNormals.clear();
        verticesNormals.insert(verticesNormals.end(), vertices.begin(), vertices.end());
        verticesNormals.insert(verticesNormals.end(), normals.begin(), normals.end());

        glGenVertexArrays(1, &this->vao);
        glGenBuffers(1, &this->vbo);

        glBindVertexArray(this->vao);

        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glBufferData(GL_ARRAY_BUFFER, verticesNormals.size() * sizeof(glm::vec3), verticesNormals.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(vertices.size() * sizeof(glm::vec3)));

        glBindVertexArray(0);


    }

    void display(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) override {
        this->display(projectionMatrix, viewMatrix, glm::vec3(0.0f, 10.0f, 20.0f));
    }


    void display(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix, const glm::vec3& cameraPos) {
        glUseProgram(shader_programme);

        
      
        double scalefactor = 6.0;
        //modelMatrix = glm::scale(modelMatrix, glm::vec3(scalefactor, scalefactor, scalefactor));
        glm::mat4 scaledModelMatrix = glm::scale(modelMatrix, glm::vec3(scalefactor, scalefactor, scalefactor));
        glm::mat4 mvp = projectionMatrix * viewMatrix * scaledModelMatrix;
        glUniformMatrix4fv(glGetUniformLocation(shader_programme, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));
        glUniformMatrix4fv(glGetUniformLocation(shader_programme, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(scaledModelMatrix));
        glUniform3fv(glGetUniformLocation(shader_programme, "viewPos"), 1, glm::value_ptr(cameraPos));
        glUniform3fv(glGetUniformLocation(shader_programme, "lightPos"), 1, glm::value_ptr(lightPos));


        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
        glBindVertexArray(0);
    }

    void setModelMatrix(glm::mat4 m) { this->modelMatrix = m; }
};