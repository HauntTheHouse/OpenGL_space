//
// Created by durda on 05.01.21.
//

#ifndef LAB3_COMP_GRAPHICS_CUBEMAP_H
#define LAB3_COMP_GRAPHICS_CUBEMAP_H

#include <vector>
#include <string>
#include "Shader.h"

class Cubemap {
private:
    std::vector<float> vertices;
    Shader *shader{};
public:
    Cubemap();
    void connectShader(const std::string &vertexPath, const std::string &fragmentPath);
    void fillVertices();
    std::vector<float> &getVertices();
    Shader getShader();
};


#endif //LAB3_COMP_GRAPHICS_CUBEMAP_H
