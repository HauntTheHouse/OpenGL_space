//
// Created by durda on 11.12.20.
//

#include "Engine.h"

#include <utility>

glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX, lastY;

Camera *(Engine::camera) = new Camera(glm::vec3(0.0f, 0.0f, 30.0f));

void Engine::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(this->width, this->height, "Usual staff", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);

    if(glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        exit(2);
    }

    glfwSetFramebufferSizeCallback(window, Engine::framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
}

void Engine::loadShaders()
{
    earth->connectShader("../shaders/earth.vert", "../shaders/earth.frag");
    sun->connectShader("../shaders/sun.vert", "../shaders/sun.frag");
    cubemap->connectShader("../shaders/cubemap.vert", "../shaders/cubemap.frag");

}

void Engine::loadTextures() {
    unsigned int texture[3];
    glGenTextures(3, texture);

    texture[0] = loadImage("../images/earth.png");
    texture[1] = loadImage("../images/night_earth.png");
    texture[2] = loadImage("../images/sun.jpg");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[2]);

    std::vector<std::string> faces
    {
        "../images/cubemap/right.jpg",
        "../images/cubemap/left.jpg",
        "../images/cubemap/top.jpg",
        "../images/cubemap/bottom.jpg",
        "../images/cubemap/front.jpg",
        "../images/cubemap/back.jpg"
    };
    cubemapTexture = loadCubemap(faces);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
}

unsigned int Engine::loadImage(std::string const &path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int imageWidth, imageHeight, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &imageWidth, &imageHeight, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int Engine::loadCubemap(std::vector<std::string> &faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int imageWidth, imageHeight, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &imageWidth, &imageHeight, &nrChannels, 0);
        if (data)
        {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, format, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void Engine::bindVertexAttributes()
{
    glGenVertexArrays(3, VAO);
    glGenBuffers(3, VBO);
    glGenBuffers(2, EBO);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, earth->getVertices().size() * sizeof(float), &earth->getVertices()[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, earth->getIndices().size() * sizeof(float), &earth->getIndices()[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sun->getVertices().size() * sizeof(float), &sun->getVertices()[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sun->getIndices().size() * sizeof(float), &sun->getIndices()[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, cubemap->getVertices().size()*sizeof(float), &cubemap->getVertices()[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
}

void Engine::loop()
{
    while(!glfwWindowShouldClose(window))
    {
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(this->window);

        glClearColor(0.0f, 0.0f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawEarth();
        drawSun();
        drawCubemap();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(3, VAO);
    glDeleteBuffers(3, VBO);
    glDeleteBuffers(2, EBO);

    glfwTerminate();
    exit(0);
}

void Engine::drawEarth()
{
    glBindVertexArray(VAO[0]);

    earth->getShader().use();
    earth->getShader().setInt("earthTexture", 0);
    earth->getShader().setInt("nightEarthTexture", 1);
    earth->getShader().setVec3("lightColor", lightColor);
    earth->getShader().setVec3("lightPos", lightPos);
    earth->getShader().setVec3("viewPos", camera->Position);


    glm::mat4 view = glm::mat4(1.0f);
    view = camera->GetViewMatrix();
    earth->getShader().setMat4("view", view);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(camera->Zoom), (float)width / (float)height, 0.001f, 100.0f);
    earth->getShader().setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians((float)-90.0), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, (float)glfwGetTime()*glm::radians((float)0.5), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(20.0f,0.0f,0.0f));
    model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, (float)glfwGetTime()*glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    earth->getShader().setMat4("model", model);

    glDrawElements(GL_TRIANGLES, earth->getIndices().size(), GL_UNSIGNED_INT, (void*)0);
}

void Engine::drawSun()
{
    glBindVertexArray(VAO[1]);

    sun->getShader().use();
    sun->getShader().setInt("sunTexture", 2);
    sun->getShader().setVec3("lightColor", lightColor);

    glm::mat4 view = glm::mat4(1.0f);
    view = camera->GetViewMatrix();
    sun->getShader().setMat4("view", view);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(camera->Zoom), (float)width / (float)height, 0.001f, 100.0f);
    sun->getShader().setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians((float)-90.0), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, (float)glfwGetTime()*glm::radians((float)1.0), glm::vec3(0.0f, 0.0f, 1.0f));


    sun->getShader().setMat4("model", model);
    glDrawElements(GL_TRIANGLES, sun->getIndices().size(), GL_UNSIGNED_INT, (void*)0);
}

void Engine::drawCubemap()
{
    glDepthFunc(GL_LEQUAL);

    glBindVertexArray(VAO[2]);

    cubemap->getShader().use();
    cubemap->getShader().setInt("space", 0);

    glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
    cubemap->getShader().setMat4("view", view);
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)width / (float)height, 0.001f, 100.0f);
    cubemap->getShader().setMat4("projection", projection);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthFunc(GL_LESS);
}

void Engine::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void Engine::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }
    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos);
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    camera->ProcessMouseMovement(xoffset, yoffset);
}

void Engine::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->ProcessMouseScroll(yoffset);
}

void Engine::processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaTime);
}

Engine::Engine(GLint w, GLint h, std::string t) : width(w), height(h), title(std::move(t)), cubemapTexture(0)
{
    earth = new Sphere(1.0f);
    sun = new Sphere(7.0f);
    cubemap = new Cubemap();

    VAO = new unsigned int(3);
    VBO = new unsigned int(3);
    EBO = new unsigned int(2);
}

Engine::~Engine()
{
    delete[] earth;
    delete[] sun;
    delete[] cubemap;

    delete[] VAO;
    delete[] VBO;
    delete[] EBO;
}
void Engine::run()
{
    init();
    loadShaders();
    loadTextures();
    bindVertexAttributes();
    loop();
}
