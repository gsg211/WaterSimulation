#pragma once
#include "Entity.h"
#include <vector>
#include <string>
#include "objloader.hpp"
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp> 
#include "math.h"

class Duck : public Entity {
private:
    GLuint textureID;
    glm::vec3 lightPos;
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<float> data;

public:
    Duck(glm::vec3 lp);

    void init() override;
    void display(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix,
                 const glm::vec3& cameraPos) override;
};
