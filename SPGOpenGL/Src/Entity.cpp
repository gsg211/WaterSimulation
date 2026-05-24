#include "Entity.h"
#include <fstream>
#include <iostream>

std::string Entity::textFileRead(const std::string& fn)
{
    std::ifstream ifile(fn);
    if (!ifile.is_open()) return "";
    return std::string((std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());
}

void Entity::load_shaders()
{
    std::string vs_s = textFileRead("shaders\\" + v_path);
    std::string fs_s = textFileRead("shaders\\" + f_path);
    const char* vs_c = vs_s.c_str();
    const char* fs_c = fs_s.c_str();

    GLint success;
    GLchar infoLog[1024];

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_c, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vs, 1024, NULL, infoLog);
        std::cerr << "VERTEX SHADER (" << v_path << "):\n" << infoLog
                  << "\n-----------------------------------------------------\n";
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_c, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fs, 1024, NULL, infoLog);
        std::cerr << "FRAGMENT SHADER (" << f_path << "):\n" << infoLog
                  << "\n-----------------------------------------------------\n";
    }

    shader_programme = glCreateProgram();
    glAttachShader(shader_programme, vs);
    glAttachShader(shader_programme, fs);
    glLinkProgram(shader_programme);
    glGetProgramiv(shader_programme, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_programme, 1024, NULL, infoLog);
        std::cerr << "LINKER:\n" << infoLog
                  << "\n-----------------------------------------------------\n";
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

Entity::Entity(std::string vertexPath, std::string fragmentPath)
{
    this->v_path = vertexPath;
    this->f_path = fragmentPath;
    this->vao = 0;
    this->vbo = 0;
    this->ebo = 0;
    this->shader_programme = 0;
}

Entity::~Entity()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    if (ebo) glDeleteBuffers(1, &ebo);
}
