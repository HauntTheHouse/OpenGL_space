//
// Created by durda on 11.12.20.
//

#ifndef LAB3_COMP_GRAPHICS_ENGINE_H
#define LAB3_COMP_GRAPHICS_ENGINE_H

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "Sphere.h"
#include "Cubemap.h"
#include "Camera.h"
#include "additional/stb_image.h"

class Engine {
private:

    unsigned int *VAO, *VBO, *EBO;
    unsigned int cubemapTexture;

    GLint width, height;
    std::string title;
    GLFWwindow* window{};
    Sphere *earth;
    Sphere *sun;
    Cubemap *cubemap;

    static Camera *camera;

    void init();
    void loadShaders();
    void loadTextures();
    void bindVertexAttributes();
    void loop();
    void drawEarth();
    void drawSun();
    void drawCubemap();
    static unsigned int loadImage(std::string const &path);
    static unsigned int loadCubemap(std::vector<std::string> &faces);

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void processInput(GLFWwindow* window);

public:
    Engine(int width, int height, const std::string &title);

    void run();
};


#endif //LAB3_COMP_GRAPHICS_ENGINE_H
