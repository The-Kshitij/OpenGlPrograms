//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <iostream>
//
//#include "Shader.h"
//
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtc/type_ptr.hpp"
//
//
//const GLchar* vertexShaderPath
//{
//	"C:/Repos/OpenGLLearn/ShaderFiles/VertexFile.txt"
//};
//
//const GLchar* fragmentShaderPath
//{
//	"C:/Repos/OpenGLLearn/ShaderFiles/FragmentFile.txt"
//};
//
//GLfloat mixValue{ 0.0f }, increment{ 0.0001f }, PerImageCoolDown{4.0f};
//GLuint VAO, VBO[2], textures[2];
//
//glm::mat4 Transform;
//
//void OnWindowReshape(GLFWwindow* window, int width, int height);
//void OnKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
//void InitVertexArrayAndBuffer();
//void InitTextures();
//void Draw(Shader shader);
//
//GLuint Indices[]
//{
//	0, 1, 2,
//	1, 2, 3,
//};
//
//GLfloat Vertices[]
//{
//	 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//
//		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
//};
//
//int main()
//{
//	if (glfwInit() == GLFW_FALSE)
//	{
//		return -1;
//	}
//
//
//	
//	int major, minor;
//	glfwGetVersion(&major, &minor, nullptr);
//	//std::cout << "OpenGL Major:" << major << ", Minor: " << minor << std::endl;
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
//
//	GLFWwindow* Window = glfwCreateWindow(600, 320, "OPEN_V_WITH_P  8-:)", nullptr, nullptr);
//	if (Window == NULL)
//	{
//		glfwTerminate();
//		return -1;
//	}
//
//	glfwMakeContextCurrent(Window);
//	
//	glewInit();
//
//	Shader shader{ vertexShaderPath, fragmentShaderPath };	
//	
//	InitVertexArrayAndBuffer();
//
//	InitTextures();
//	
//	
//	//Transform = glm::rotate(Transform, 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
//
//	glfwSetWindowSizeCallback(Window, OnWindowReshape);
//
//	while (!glfwWindowShouldClose(Window)) 
//	{
//		glfwPollEvents();		
//		Draw(shader);
//		glfwSwapBuffers(Window);
//	}
//		
//	glDeleteBuffers(1, VBO);	
//	glfwTerminate();
//	return 0;
//}
//
//void OnWindowReshape(GLFWwindow* window, int width, int height)
//{
//	glViewport(0, 0, width, height);
//}
//
//void OnKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//
//	if (action == GLFW_PRESS)
//	{
//		switch (key)
//		{
//		case GLFW_KEY_UP:
//			mixValue = (mixValue + 0.1f) > 1.0f ? 1.0f : (mixValue + 0.1f);
//			break;
//		case GLFW_KEY_DOWN:
//			mixValue = (mixValue - 0.1f) < 0.0f ? 0.0f : (mixValue - 0.1f);
//			break;
//		}
//	}
//}
//
//void InitVertexArrayAndBuffer()
//{
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(2, VBO);
//
//	glBindVertexArray(VAO);
//		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[1]);
//		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
//
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
//		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//
//		glEnableVertexAttribArray(0);
//		glEnableVertexAttribArray(1);
//
//	glBindVertexArray(0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//}
//
//void InitTextures()
//{
//	GLint x, y, comps;
//	glGenTextures(2, textures);
//
//	const unsigned char* image_1{ stbi_load("bg.jpg", &x, &y, &comps, STBI_rgb) };
//	std::cout << "Loaded image 1 with x{" << x << "}, y{" << y << "}, components{" << comps << "}"<<std::endl;	
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, textures[0]);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// Set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, image_1);
//	glGenerateMipmap(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	const unsigned char* image_2{ stbi_load("myface.jpg", &x, &y, &comps, STBI_rgb) };
//	std::cout << "Loaded image 2 with x{" << x << "}, y{" << y << "}, components{" << comps << "}" << std::endl;
//
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, textures[1]);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// Set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, image_2);
//	glGenerateMipmap(GL_TEXTURE_2D);
//
//	glBindTexture(GL_TEXTURE_2D, 0);	
//}
//
//void Draw(Shader shader)
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//	
//	glBindVertexArray(VAO);
//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, textures[0]);
//	glUniform1i(shader.GetUniformLocationFromProgram("texture1"), 0);
//
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, textures[1]);
//	glUniform1i(shader.GetUniformLocationFromProgram("texture2"), 1);	
//
//	glUniform1f(shader.GetUniformLocationFromProgram("MixValue"), mixValue);
//	glUniformMatrix4fv(shader.GetUniformLocationFromProgram("transform"), 1, 
//		GL_FALSE, glm::value_ptr(Transform));
//	mixValue += increment;
//
//	if (mixValue > 1.0f)
//	{
//		mixValue = 1.0f;
//		if (PerImageCoolDown > 0.0f)
//		{
//			PerImageCoolDown -= 0.001f;
//		}
//
//		if (PerImageCoolDown < 0.0f)
//		{
//			PerImageCoolDown = 4.0f;
//			increment = -increment;
//		}
//	}
//	else if (mixValue < 0.0f)
//	{
//		mixValue = 0.0f;
//		if (PerImageCoolDown > 0.0f)
//		{
//			PerImageCoolDown -= 0.001f;
//		}
//
//		if (PerImageCoolDown < 0.0f)
//		{
//			PerImageCoolDown = 4.0f;
//			increment = -increment;
//		}
//	}
//	shader.UseProgram();
//}