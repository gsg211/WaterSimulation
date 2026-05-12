#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include "camera.h"
#include "Water.h" 
#include "skybox.h"
#include "buoy.h"
Camera* camera;

Water* water;
Skybox* skybox;
Buoy* buoy;

glm::mat4 projectionMatrix, modelMatrix;
glm::vec3 lightPos(-100.0f, 50.0f, 0.0f);



void init() {
    glewInit();

    camera = new Camera();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.549f, 1.0f, 1.0f);


    std::vector<std::string> faces = {
        "textures/front.png", "textures/back.png", "textures/top.png", "textures/bottom.png", "textures/right.png", "textures/left.png"
    };

    skybox = new Skybox(faces);
    skybox->init();

    water = new Water(900, 300.0f, lightPos);
    water->setSkyboxId(skybox->getTextureID());
    water->init();
    buoy = new Buoy(lightPos);

    buoy->init();


}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera->update();
    glm::mat4 viewMatrix = camera->getViewMatrix();
    skybox->display(projectionMatrix, viewMatrix);
    water->display(projectionMatrix, viewMatrix, camera->pos);
    buoy->display(projectionMatrix, viewMatrix, camera->pos);

    glutPostRedisplay();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float)w / h, 0.1f, 1000.0f);
}

void mouse_callback(int x, int y) { camera->handleMouse(x, y); }
void kbD(unsigned char k, int x, int y) { camera->handleKeyboard(k, true); if (k == 27) exit(0); }
void kbU(unsigned char k, int x, int y) { camera->handleKeyboard(k, false); }


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