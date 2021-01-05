//
// Created by durda on 11.12.20.
//

#include "HandleInput.h"

void HandleInput::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}