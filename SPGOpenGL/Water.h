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
    glm::vec3 lightPos;

    std::vector<float> generateWaterMesh() {
        std::vector<float> vertices;
        float step = size / resolution;
        float offset = size / 2.0f;
        for (int i = 0; i < resolution; i++) {
            for (int j = 0; j < resolution; j++) {
                float x = i * step - offset;
                float z = j * step - offset;

                float p[6][6] = {
                    {x, 0, z, 0, 1, 0}, {x + step, 0, z, 0, 1, 0}, {x, 0, z + step, 0, 1, 0},
                    {x + step, 0, z, 0, 1, 0}, {x + step, 0, z + step, 0, 1, 0}, {x, 0, z + step, 0, 1, 0}
                };
                for (int k = 0; k < 6; k++) {
                    for (int l = 0; l < 6; l++) {
                        vertices.push_back(p[k][l]);
                    }
                }
            }
        }
        return vertices;
    }

public:
    Water(int res, float s) : Entity("WaterVertex.vert", "WaterFragment.frag"), resolution(res), size(s) {
        lightPos = glm::vec3(0.0f, 50.0f, 0.0f); 
    }

    void init() override {
        load_shaders();
        std::vector<float> data = generateWaterMesh();
        nrVertices = (int)data.size() / 6;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

        glBindVertexArray(0);
    }

    void display(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix, const glm::vec3& cameraPos) {
        glUseProgram(shader_programme);

        float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
        glm::mat4 modelMatrix = glm::mat4(1.0f);

        glUniform1f(glGetUniformLocation(shader_programme, "time"), time);
        glUniformMatrix4fv(glGetUniformLocation(shader_programme, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * modelMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shader_programme, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(modelMatrix))));

        glUniform3fv(glGetUniformLocation(shader_programme, "viewPos"), 1, glm::value_ptr(cameraPos));
        glUniform3fv(glGetUniformLocation(shader_programme, "lightPos"), 1, glm::value_ptr(lightPos));

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, nrVertices);
        glBindVertexArray(0);
    }

    void display(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) override {
        this->display(projectionMatrix, viewMatrix, glm::vec3(0.0f));
    }
};