#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class IEntity {
public:
    virtual ~IEntity() {}
    virtual void init() = 0;
    virtual void display(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix,
                         const glm::vec3& cameraPos = glm::vec3(0.0f, 0.0f, 0.0f)) = 0;
};

class Entity : public IEntity {
protected:
    GLuint vao, vbo, ebo;
    GLuint shader_programme;
    std::string v_path, f_path;

    std::string textFileRead(const std::string& fn);
    void load_shaders();

public:
    Entity(std::string vertexPath, std::string fragmentPath);
    virtual ~Entity();
};
