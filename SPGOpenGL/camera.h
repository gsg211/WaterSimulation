#pragma once
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    bool keys[256];
    float lastX, lastY;
    bool firstMouse;

    Camera(glm::vec3 startPos = glm::vec3(0.0f, 5.0f, 20.0f));

    glm::mat4 getViewMatrix();
    void handleKeyboard(unsigned char key, bool isPressed);
    void handleMouse(int x, int y);
    void update();

private:
    void updateVectors();
};
