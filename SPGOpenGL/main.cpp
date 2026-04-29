#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#define PI glm::pi<float>()

// Configurații Water Mesh
const int resolution = 900;
const float terrainSize = 400.0f; 
int nrVertices;
GLuint shader_programme, vao;

glm::mat4 projectionMatrix, viewMatrix, modelMatrix;

glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 20.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -0.2f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f, pitch = -15.0f;

float lastX = 400, lastY = 300;
bool firstMouse = true;
float cameraSpeed = 0.4f;
bool keys[256];


glm::vec3 lightPos(0.0f, 20.0f, 0.0f);

std::vector<float> generateWaterMesh(int res, float size) {

    std::vector<float> vertices;
    float step = size / res;
    float offset = size / 2.0f;

    for (int i = 0; i < res; i++) {
        for (int j = 0; j < res; j++) {

            float x = i * step - offset;
            float z = j * step - offset;
            float p[6][6] = {
                {x, 0, z, 0, 1, 0}, {x + step, 0, z, 0, 1, 0}, {x, 0, z + step, 0, 1, 0},
                {x + step, 0, z, 0, 1, 0}, {x + step, 0, z + step, 0, 1, 0}, {x, 0, z + step, 0, 1, 0}
            };
            for (int k = 0; k < 6; k++) for (int l = 0; l < 6; l++) vertices.push_back(p[k][l]);
        }
    }

    return vertices;
}



std::string textFileRead(const char* fn) {
    std::ifstream ifile(fn);
    std::string text((std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());
    return text;
}

void mouse_callback(int x, int y) {
    if (firstMouse) { lastX = x; lastY = y; firstMouse = false; }

    float xoffset = x - lastX;
    float yoffset = lastY - y;

    lastX = x; lastY = y;
    float sens = 0.1f;

    yaw += xoffset * sens;
    pitch += yoffset * sens;

    if (pitch > 89.0f) pitch = 89.0f; if (pitch < -89.0f) pitch = -89.0f;
    glm::vec3 front;

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(front);
}

void updateCamera() {

    glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));
    if (keys['w']) cameraPos += cameraFront * cameraSpeed;
    if (keys['s']) cameraPos -= cameraFront * cameraSpeed;
    if (keys['a']) cameraPos -= right * cameraSpeed;
    if (keys['d']) cameraPos += right * cameraSpeed;

    if (keys['q']) cameraPos += cameraUp * cameraSpeed;
    if (keys['e']) cameraPos -= cameraUp * cameraSpeed;

}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_programme);
    updateCamera();

    viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    modelMatrix = glm::mat4(1.0f);

    glUniform1f(glGetUniformLocation(shader_programme, "time"), glutGet(GLUT_ELAPSED_TIME) / 1000.0f);

    glUniformMatrix4fv(glGetUniformLocation(shader_programme, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader_programme, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(modelMatrix))));
   
    glUniform3fv(glGetUniformLocation(shader_programme, "viewPos"), 1, glm::value_ptr(cameraPos));
    glUniform3fv(glGetUniformLocation(shader_programme, "lightPos"), 1, glm::value_ptr(lightPos));

    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, nrVertices);
    glutPostRedisplay();
    glutSwapBuffers();
}

void init() {
    glewInit();
   
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    std::string vs_s = textFileRead("vertex.vert"), fs_s = textFileRead("fragment.frag");
    const char* vs_c = vs_s.c_str(), * fs_c = fs_s.c_str();
   
    GLuint vs = glCreateShader(GL_VERTEX_SHADER); glShaderSource(vs, 1, &vs_c, NULL); glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER); glShaderSource(fs, 1, &fs_c, NULL); glCompileShader(fs);
    
    shader_programme = glCreateProgram(); glAttachShader(shader_programme, vs); glAttachShader(shader_programme, fs); glLinkProgram(shader_programme);

    std::vector<float> data = generateWaterMesh(resolution, terrainSize);
    nrVertices = data.size() / 6;

    GLuint vbo; glGenBuffers(1, &vbo); glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
    glGenVertexArrays(1, &vao); glBindVertexArray(vao);

    glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    glEnableVertexAttribArray(1); glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float)w / h, 0.1f, 1000.0f);
}

void kbD(unsigned char k, int x, int y) { keys[k] = true; if (k == 27) exit(0); }
void kbU(unsigned char k, int x, int y) { keys[k] = false; }

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(1200, 800);

    glutCreateWindow("proiect spg");
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(kbD);
    glutKeyboardUpFunc(kbU);

    glutPassiveMotionFunc(mouse_callback);
    glutSetCursor(GLUT_CURSOR_NONE); 

    glutMainLoop();
    return 0;
}