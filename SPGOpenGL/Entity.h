#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class IEntity {
public:
    virtual ~IEntity() {}
    virtual void init() = 0;
    virtual void display(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) = 0;
};


class Entity : public IEntity {
protected:
    GLuint vao, vbo, ebo;
    GLuint shader_programme;

    std::string v_path, f_path;

    std::string textFileRead(const std::string& fn) {
        std::ifstream ifile(fn);
        if (!ifile.is_open()) return "";
        return std::string((std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());
    }

    void load_shaders() {
        std::string vs_s = textFileRead(v_path);
        std::string fs_s = textFileRead(f_path);
        const char* vs_c = vs_s.c_str();
        const char* fs_c = fs_s.c_str();

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vs_c, NULL);
        glCompileShader(vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fs_c, NULL);
        glCompileShader(fs);

        shader_programme = glCreateProgram();
        glAttachShader(shader_programme, vs);
        glAttachShader(shader_programme, fs);
        glLinkProgram(shader_programme);

        glDeleteShader(vs);
        glDeleteShader(fs);
    }

public:
    Entity(std::string vertexPath, std::string fragmentPath) {
        this->v_path = vertexPath;
        this->f_path = fragmentPath;
        this->vao = 0;
        this->vbo = 0;
        this->ebo = 0;
    }
        
    virtual ~Entity() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        if (ebo) glDeleteBuffers(1, &ebo);
    }
};