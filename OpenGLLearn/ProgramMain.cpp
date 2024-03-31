/*
* directional light
* point light
* spot light
*/
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// GLM Mathematics
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"

GLfloat XDegree{0.0f}, YDegree{ 0.0f }, ZDegree{ 0.0f };

GLuint SceneProgram, LightSourceProgram;
GLint Width{ 900 }, Height{ 900 }, NumChannels;

glm::mat4 ModelMatrix, LightSourceModelMatrix, ViewMatrix, ProjectionMatrix;

GLfloat mixValue{0.0f};

GLuint VAOs[2], VBO;

void GetLastError();
void KeyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void OnResize(GLFWwindow* window, int width, int height);

int main()
{
    if (GLFW_FALSE == glfwInit())
    {
        GetLastError();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* windowPtr = glfwCreateWindow(Width, Height, ":--)", nullptr, nullptr);
    if (windowPtr == nullptr)
    {
        GetLastError();
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(windowPtr);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "glewinit failed";
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(windowPtr, KeyPressedCallback);        

    GLuint textures[2];
    glGenTextures(2, textures);

    int x, y;
    const unsigned char* objectDiff{ stbi_load("Textures/container_diff.png", &x, &y, nullptr, 3) };    
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, objectDiff);
    glGenerateMipmap(GL_TEXTURE_2D);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    

    const unsigned char* objectSpec{ stbi_load("Textures/container_specular.png", &x, &y, nullptr, 3) };
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, objectSpec);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -3.0f);
    ViewMatrix = glm::translate(ViewMatrix, cameraPos);
    ProjectionMatrix = glm::perspective(90.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 500.0f);
    glm::vec3 lightPosition = glm::vec3(-0.4f, 0.65f, 0.65f);
    LightSourceModelMatrix = glm::translate(LightSourceModelMatrix, lightPosition);
    LightSourceModelMatrix = glm::scale(LightSourceModelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));

    Shader objectShader{ "ShaderFiles/VertexFile.txt",
        "ShaderFiles/ModelFragmentFile.txt", "Model Shader Vertex", "Model Shader Fragment" };
    const GLuint objectShaderModelMatrix = objectShader.GetUniformLocationFromProgram("ModelMatrix");
    const GLuint objectShaderViewMatrix = objectShader.GetUniformLocationFromProgram("ViewMatrix");
    const GLuint objectShaderProjectionMatrix = objectShader.GetUniformLocationFromProgram("ProjectionMatrix");
    const GLuint objectShaderDiffuse = objectShader.GetUniformLocationFromProgram("ObjectMaterial.DiffuseMap");
    const GLuint objectShaderSpecular = objectShader.GetUniformLocationFromProgram("ObjectMaterial.SpecularMap");
    const GLuint objectShaderLightPos = objectShader.GetUniformLocationFromProgram("LightPos");
    const GLuint objectShaderViewLocation = objectShader.GetUniformLocationFromProgram("ViewLocation");
    const GLuint objectShaderLightambience = objectShader.GetUniformLocationFromProgram("LightProperties.ambience");
    const GLuint objectShaderLightshine = objectShader.GetUniformLocationFromProgram("LightProperties.shine");

    objectShader.UseProgram();    
    glUniformMatrix4fv(objectShaderProjectionMatrix, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
    glUniform3f(objectShaderViewLocation, cameraPos.x, cameraPos.y, cameraPos.z);
    glUniform3f(objectShaderLightPos, lightPosition.x, lightPosition.y, lightPosition.z);
    glUniform1i(objectShaderDiffuse, 0);
    glUniform1i(objectShaderSpecular, 1);
    glUniform1f(objectShaderLightambience, 0.2f);
    glUniform1f(objectShaderLightshine, 64);
       
    Shader lightSourceShader{ "ShaderFiles/VertexFile.txt",
        "ShaderFiles/LightFragmentFile.txt", "Light Source Vertex", "Light Source Fragment"};
    const GLuint lightSourceShaderModelMatrix = lightSourceShader.GetUniformLocationFromProgram("ModelMatrix");
    const GLuint lightSourceShaderViewMatrix = lightSourceShader.GetUniformLocationFromProgram("ViewMatrix");
    const GLuint lightSourceShaderProjectionMatrix = lightSourceShader.GetUniformLocationFromProgram("ProjectionMatrix");
    lightSourceShader.UseProgram();
    glUniformMatrix4fv(lightSourceShaderProjectionMatrix, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
    glUniformMatrix4fv(lightSourceShaderModelMatrix, 1, GL_FALSE, glm::value_ptr(LightSourceModelMatrix));


    GLfloat Vertices[] =
    {
        // Positions          // Normals           // Texture Coords
             -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
              0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
              0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
              0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
             -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
             -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

             -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
              0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
              0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
              0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
             -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

             -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
             -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
             -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

              0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
              0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
              0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
              0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
              0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
              0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

             -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
              0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
              0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
              0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
             -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
             -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

             -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
              0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
              0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
              0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
             -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
             -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    };

    glGenVertexArrays(2, VAOs);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glViewport(0, 0, Width, Height);
    while (!glfwWindowShouldClose(windowPtr))
    {        
        glfwPollEvents();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        objectShader.UseProgram();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glUniformMatrix4fv(objectShaderModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
        glUniformMatrix4fv(objectShaderViewMatrix, 1, GL_FALSE, glm::value_ptr(ViewMatrix));

        lightSourceShader.UseProgram();
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glUniformMatrix4fv(lightSourceShaderViewMatrix, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
        

        glfwSwapBuffers(windowPtr);        
    }
    
    glfwTerminate();
    return 0;
}

void GetLastError()
{
    const char* error;
    glfwGetError(&error);
    std::cout << error;
}



void KeyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS))
    {
        //std::cout << "Escape Key Pressed \n";
        glfwSetWindowShouldClose(window, GL_TRUE);
    }    

    else if ((key == GLFW_KEY_I) && (action == GLFW_PRESS))
    {        
        mixValue += 0.1f;
        mixValue = mixValue > 1.0f ? 1.0f : mixValue;        
    }
    else if ((key == GLFW_KEY_O) && (action == GLFW_PRESS))
    {        
        mixValue -= 0.1f;
        mixValue = mixValue < 0.0f ? 0.0f : mixValue;        
    }
    else if ((key == GLFW_KEY_W) && (action == GLFW_PRESS))
    {                
        ViewMatrix = glm::translate(ViewMatrix, glm::vec3(0, 0, 1));
    }
    else if ((key == GLFW_KEY_S) && (action == GLFW_PRESS))
    {
        ViewMatrix = glm::translate(ViewMatrix, glm::vec3(0, 0, -1));                     
    }
    else if ((key == GLFW_KEY_A) && (action == GLFW_PRESS))
    {
        ViewMatrix = glm::translate(ViewMatrix, glm::vec3(1, 0, 0));
    }
    else if ((key == GLFW_KEY_D) && (action == GLFW_PRESS))
    {
        ViewMatrix = glm::translate(ViewMatrix, glm::vec3(-1, 0, 0));
    }
    else if ((key == GLFW_KEY_Q) && (action == GLFW_PRESS))
    {
        ViewMatrix = glm::translate(ViewMatrix, glm::vec3(0, 1, 0));
    }
    else if ((key == GLFW_KEY_E) && (action == GLFW_PRESS))
    {
        ViewMatrix = glm::translate(ViewMatrix, glm::vec3(0, -1, 0));
    }
    else if ((key == GLFW_KEY_RIGHT) && (action == GLFW_PRESS))
    {
        ModelMatrix = glm::rotate(ModelMatrix, -10.0f, glm::vec3(0, 1, 0));
    }
    else if ((key == GLFW_KEY_LEFT) && (action == GLFW_PRESS))
    {
        ModelMatrix = glm::rotate(ModelMatrix, 10.0f, glm::vec3(0, 1, 0));        
    }
    else if ((key == GLFW_KEY_UP) && (action == GLFW_PRESS))
    {
        ModelMatrix = glm::rotate(ModelMatrix, -10.0f, glm::vec3(1, 0, 0));
        XDegree -= 10.0f;
        std::cout << "X degree: " << XDegree << std::endl;
    }
    else if ((key == GLFW_KEY_DOWN) && (action == GLFW_PRESS))
    {
        ModelMatrix = glm::rotate(ModelMatrix, 10.0f, glm::vec3(1, 0, 0));
        XDegree += 10.0f;
        std::cout << "X degree: " << XDegree << std::endl;
    }
}

void OnResize(GLFWwindow* window, int width, int height)
{
    Width = width;
    Height = height;
    glViewport(0, 0, Width, Height);    
}