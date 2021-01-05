//
// Created by durda on 11.12.20.
//

#ifndef LAB3_COMP_GRAPHICS_SPHERE_H
#define LAB3_COMP_GRAPHICS_SPHERE_H

#include <vector>
#include <cmath>
#include <string>
#include "Shader.h"

class Sphere {
private:
    float radius{};
    unsigned int sectorCount{};
    unsigned int stackCount{};

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    Shader *shader{};
public:
    Sphere();
    Sphere(const float &radius, const unsigned int &sectorCount = 50, const unsigned int &stackCount = 50);

    void connectShader(const std::string &vertexPath, const std::string &fragmentPath);
    void fillVertices();
    void fillIndices();
    std::vector<float> &getVertices();
    std::vector<unsigned int> &getIndices();
    Shader getShader();
};

#endif //LAB3_COMP_GRAPHICS_SPHERE_H
