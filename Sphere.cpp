//
// Created by durda on 11.12.20.
//

#include "Sphere.h"


Sphere::Sphere() = default;
Sphere::Sphere(const float &radius, const unsigned int &sectorCount, const unsigned int &stackCount)
{
    this->radius = radius;
    this->sectorCount = sectorCount;
    this->stackCount = stackCount;

    fillVertices();
    fillIndices();
}

void Sphere::connectShader(const std::string &vertexPath, const std::string &fragmentPath)
{
    shader = new Shader(vertexPath.c_str(), fragmentPath.c_str());
}

void Sphere::fillVertices()
{
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / this->radius;
    float s, t;

    float sectorStep = 2.0 * M_PI / (float)this->sectorCount;
    float stackStep = M_PI / (float)this->stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= this->stackCount; ++i)
    {
        stackAngle = M_PI / 2.0 - (float)i * stackStep;
        xy = this->radius * cos(stackAngle);
        z = this->radius * sin(stackAngle);
        for(int j = 0; j <= this->sectorCount; ++j)
        {
            sectorAngle = (float)j * sectorStep;
            x = xy * cos(sectorAngle);
            y = xy * sin(sectorAngle);

            this->vertices.push_back(x);
            this->vertices.push_back(y);
            this->vertices.push_back(z);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            this->vertices.push_back(nx);
            this->vertices.push_back(ny);
            this->vertices.push_back(nz);

            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            this->vertices.push_back(s);
            this->vertices.push_back(t);
        }
    }
}

void Sphere::fillIndices()
{
    int k1, k2;

    for(int i = 0; i < this->stackCount; ++i)
    {
        k1 = i * (this->sectorCount + 1);
        k2 = k1 + this->sectorCount + 1;

        for (int j = 0; j < this->sectorCount; ++j, ++k1, ++k2) {
            if(i != 0)
            {
                this->indices.push_back(k1);
                this->indices.push_back(k2);
                this->indices.push_back(k1 + 1);
            }
            if(i != this->stackCount - 1)
            {
                this->indices.push_back(k1 + 1);
                this->indices.push_back(k2);
                this->indices.push_back(k2 + 1);
            }
        }
    }
}

std::vector<float> &Sphere::getVertices()
{
    return this->vertices;
}
std::vector<unsigned int> &Sphere::getIndices()
{
    return this->indices;
}

Shader Sphere::getShader()
{
    return *shader;
}