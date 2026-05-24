#include "Skybox.h"
#include <iostream>

Skybox::Skybox(std::vector<std::string> facesPaths)
    : Entity("SkyboxVertex.vert", "SkyboxFragment.frag"), faces(facesPaths)
{
}

void Skybox::init()
{
    load_shaders();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    char cwd[1024];
    _getcwd(cwd, sizeof(cwd));
    std::cout << "The program is looking in: " << cwd << std::endl;

    // loading the texture images
    for (unsigned int i = 0; i < faces.size(); i++) {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 3);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                         width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
    }

    // interpolation
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // when texture smaller than pixel -> linear interpolation
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // when texture larger than pixel -> linear interpolation

    // clamping
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // U: horizontal
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // V: vertical
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // W: depth
}

void Skybox::display(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix,
                     const glm::vec3& cameraPos)
{
    // draws object if z <= depth buffer
    // skybox z is 1.0
    glDepthFunc(GL_LEQUAL);
    glUseProgram(shader_programme);

    // remove translation from view matrix so skybox stays static
    glm::mat4 staticView = glm::mat4(glm::mat3(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader_programme, "view"),       1, GL_FALSE, glm::value_ptr(staticView));
    glUniformMatrix4fv(glGetUniformLocation(shader_programme, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // turn the depth function back to normal
    glDepthFunc(GL_LESS);
}

GLuint Skybox::getTextureID() const
{
    return textureID;
}
