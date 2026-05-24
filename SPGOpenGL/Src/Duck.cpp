#include "Duck.h"
#include <iostream>
#include <direct.h>
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

Duck::Duck(glm::vec3 lp)
    : Entity("DuckVertex.vert", "DuckFragment.frag"), lightPos(lp)
{
    textureID = 0;
}

void Duck::init()
{
    load_shaders();

    if (!loadOBJ("obj/duck.obj", vertices, uvs, normals)) {
        printf("Failed to load OBJ!\n");
    }

    std::string texturePath = "textures\\duck.jpg";

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

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // if coord out of texture -> repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // uses mipmaps from distance
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // interpolation when near
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

void Duck::display(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix,
                   const glm::vec3& cameraPos)
{
    glUseProgram(shader_programme);

    float time = (float)glutGet(GLUT_ELAPSED_TIME) / 500.0f;
    double scalefactor = 0.11;
    float amount = sin(time) / 1.5f - 1.5f;

    // floating
    glm::mat4 translated = glm::translate(modelMatrix, glm::vec3(0.0, amount, 0.0));

    // upright rotation
    glm::mat4 rotated = glm::rotate(translated, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // left right rotation
    rotated *= glm::rotate(glm::mat4(1.0), glm::radians(sin(time) * 3 + 3), glm::vec3(0.0f, 1.0f, 0.0f));
    // front back rotation
    rotated *= glm::rotate(glm::mat4(1.0), glm::radians(sin(time) * 3 + 10), glm::vec3(1.0f, 0.0f, 0.0f));
    // around its own axis
    rotated *= glm::rotate(glm::mat4(1.0), glm::radians(time), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 scaledModelMatrix = glm::scale(rotated, glm::vec3(scalefactor));
    glm::mat4 mvp = projectionMatrix * viewMatrix * scaledModelMatrix;

    glUniformMatrix4fv(glGetUniformLocation(shader_programme, "mvpMatrix"),    1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(glGetUniformLocation(shader_programme, "modelMatrix"),  1, GL_FALSE, glm::value_ptr(scaledModelMatrix));
    glUniform3fv(glGetUniformLocation(shader_programme, "viewPos"),  1, glm::value_ptr(cameraPos));
    glUniform3fv(glGetUniformLocation(shader_programme, "lightPos"), 1, glm::value_ptr(lightPos));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shader_programme, "textureSampler"), 0);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
    glBindVertexArray(0);
}
