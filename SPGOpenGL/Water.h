#pragma once
#include "Entity.h"
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Water : public Entity {
private:
    int resolution; //nr of squares
    float size; //size of mesh
    int nrVertices;
    GLuint skyboxID;
    glm::vec3 lightPos;

    std::vector<float> generateWaterMesh() {
        std::vector<float> vertices;
        float squareSize = size / resolution; //size of square
        float offset = size / 2.0f; //center origin

        for (int i = 0; i < resolution; i++) {
            for (int j = 0; j < resolution; j++) {
                float x = i * squareSize - offset;
                float z = j * squareSize - offset;

                //generate square
                float p[6][3] = {
                    {x, 0, z}, {x + squareSize, 0, z}, {x, 0, z + squareSize},
                    {x + squareSize, 0, z}, {x + squareSize, 0, z + squareSize}, {x, 0, z + squareSize}
                };

                //add current square to vertices
                for (int k = 0; k < 6; k++) {
                    for (int l = 0; l < 3; l++) {
                        vertices.push_back(p[k][l]);
                    }
                }
            }
        }
        return vertices;
    }

public:
    Water(int res, float s, glm::vec3 lightpos) : Entity("WaterVertex.vert", "WaterFragment.frag"), resolution(res), size(s) {
        this->lightPos = lightpos;
    }

    void init() override {
        load_shaders();


        std::vector<float> data = generateWaterMesh();
        nrVertices = (int)data.size() / 3; 

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glBindVertexArray(0);
    }

    void display(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix, const glm::vec3& cameraPos) {
        glUseProgram(shader_programme);

        float time = (float)glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
        glm::mat4 modelMatrix = glm::mat4(1.0f);

        //time used for animation
        glUniform1f(glGetUniformLocation(shader_programme, "time"), time);

        //skybox (cubemap) for reflexion
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID);
        glUniform1i(glGetUniformLocation(shader_programme, "skybox"), 0);

        //mvp + viewpos + lightpos
        glUniformMatrix4fv(glGetUniformLocation(shader_programme, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * modelMatrix));
        glUniform3fv(glGetUniformLocation(shader_programme, "viewPos"), 1, glm::value_ptr(cameraPos));
        glUniform3fv(glGetUniformLocation(shader_programme, "lightPos"), 1, glm::value_ptr(lightPos));

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, nrVertices);
        glBindVertexArray(0);
    }



    void setSkyboxId(GLuint skybox) {
        this->skyboxID = skybox;
    }
};