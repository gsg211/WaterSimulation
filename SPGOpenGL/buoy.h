#pragma once
#include "Entity.h"
#include <vector>
#include <string>
#include <iostream>
#include "objloader.hpp"
#include <direct.h>
#include "stb_image.h"
#include "mtlloader.h"


class Buoy : public Entity {
private:
    GLuint textureID;
    glm::vec3 lightPos;
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<float> data;

public:
    Buoy(glm::vec3 lp) : Entity("BuoyVertex.vert", "BuoyFragment.frag"), lightPos(lp) {
        textureID = 0; // Fixed warning
    }

    void init() override {
        load_shaders();

        // 1. Load OBJ
        if (!loadOBJ("obj/buoy.obj", vertices, uvs, normals)) {
            printf("Failed to load OBJ!\n");
        }

        // 2. Parse the MTL to find the real texture filename
        auto materials = loadMTL("buoy.mtl");
        std::string texturePath = "textures/buoy_diffuse.png"; // safe fallback

        if (!materials.empty()) {
            // grab the first material's diffuse texture
            const auto& mat = materials.begin()->second;
            if (!mat.diffuseTexture.empty()) {
                // MTL paths are relative to the .mtl file location
                texturePath =  mat.diffuseTexture;
                printf("Using texture from MTL: %s\n", texturePath.c_str());
            }
        }

        // 3. VAO / VBOs (unchanged)
        glGenVertexArrays(1, &this->vao);
        glBindVertexArray(this->vao);

        GLuint posVBO;
        glGenBuffers(1, &posVBO);
        glBindBuffer(GL_ARRAY_BUFFER, posVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        GLuint normalVBO;
        glGenBuffers(1, &normalVBO);
        glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        GLuint uvVBO;
        glGenBuffers(1, &uvVBO);
        glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindVertexArray(0);

        // 4. Load texture from the path found in MTL
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(false);

        unsigned char* texData = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
        if (texData) {
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texData);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(texData);
            printf("Texture loaded OK: %dx%d, %d channels\n", width, height, nrChannels);
        }
        else {
            printf("Failed to load texture: %s\nSTB reason: %s\n", texturePath.c_str(), stbi_failure_reason());
        }
    }

    void display(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) override {
        this->display(projectionMatrix, viewMatrix, glm::vec3(0.0f, 10.0f, 20.0f));
    }

    void display(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix, const glm::vec3& cameraPos) {
        glUseProgram(shader_programme);

        double scalefactor = 6.0;
        glm::mat4 scaledModelMatrix = glm::scale(modelMatrix, glm::vec3(scalefactor, scalefactor, scalefactor));
        glm::mat4 mvp = projectionMatrix * viewMatrix * scaledModelMatrix;

        glUniformMatrix4fv(glGetUniformLocation(shader_programme, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));
        glUniformMatrix4fv(glGetUniformLocation(shader_programme, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(scaledModelMatrix));
        glUniform3fv(glGetUniformLocation(shader_programme, "viewPos"), 1, glm::value_ptr(cameraPos));
        glUniform3fv(glGetUniformLocation(shader_programme, "lightPos"), 1, glm::value_ptr(lightPos));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(glGetUniformLocation(shader_programme, "textureSampler"), 0);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
        glBindVertexArray(0);
    }

    void setModelMatrix(glm::mat4 m) { this->modelMatrix = m; }
};