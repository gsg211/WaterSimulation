#include "Camera.h"

Camera::Camera(glm::vec3 startPos)
{
    this->pos = startPos;
    this->front = glm::vec3(0.0f, -0.2f, -1.0f);
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->yaw = -90.0f;
    this->pitch = -15.0f;
    this->speed = 0.4f;
    this->sensitivity = 0.1f;
    this->lastX = 400.0f;
    this->lastY = 300.0f;
    this->firstMouse = true;
    for (int i = 0; i < 256; i++) {
        this->keys[i] = false;
    }
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(pos, pos + front, up);
}

void Camera::handleKeyboard(unsigned char key, bool isPressed)
{
    keys[key] = isPressed;
}

void Camera::handleMouse(int x, int y)
{
    int centerX = 400;
    int centerY = 300;

    if (firstMouse) {
        lastX = (float)x;
        lastY = (float)y;
        firstMouse = false;
    }

    float xoffset = (float)x - lastX;
    float yoffset = lastY - (float)y;
    lastX = (float)x;
    lastY = (float)y;

    yaw   += xoffset * sensitivity;
    pitch += yoffset * sensitivity;

    if (pitch >  89.0f) pitch =  89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateVectors();

    if (x < 100 || x > 700 || y < 100 || y > 500) {
        lastX = (float)centerX;
        lastY = (float)centerY;
        glutWarpPointer(centerX, centerY);
    }
}

void Camera::update()
{
    glm::vec3 right = glm::normalize(glm::cross(front, up));
    if (keys['w']) pos += front * speed;
    if (keys['s']) pos -= front * speed;
    if (keys['a']) pos -= right * speed;
    if (keys['d']) pos += right * speed;
    if (keys['q']) pos += up * speed;
    if (keys['e']) pos -= up * speed;
}

void Camera::updateVectors()
{
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw))   * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw))   * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
}
