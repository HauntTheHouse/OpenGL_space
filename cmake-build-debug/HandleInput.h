//
// Created by durda on 11.12.20.
//

#ifndef LAB3_COMP_GRAPHICS_HANDLEINPUT_H
#define LAB3_COMP_GRAPHICS_HANDLEINPUT_H

#include <GLFW/glfw3.h>
#include "Cumera.h"

class HandleInput {
public:
    static Camera *camera;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void processInput(GLFWwindow* window);

    static void setCamera(Camera *cam);
};

#endif //LAB3_COMP_GRAPHICS_HANDLEINPUT_H
