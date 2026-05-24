#pragma once
#include "Entity.h"
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Water : public Entity {
private:
    int resolution;
    float size;
    int nrVertices;
    GLuint skyboxID;
    glm::vec3 lightPos;

    std::vector<float> generateWaterMesh();

public:
    Water(int res, float s, glm::vec3 lightpos);

    void init() override;
    void display(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix,
                 const glm::vec3& cameraPos) override;
    void setSkyboxId(GLuint skybox);
};
