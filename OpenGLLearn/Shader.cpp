#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#define MaxInfoLength 512

Shader::Shader(const GLchar* vertexShaderFilePath, const GLchar* fragmentShaderFilePath, const GLchar* VertexShaderName, const GLchar* FragmentShaderName)
{	
	if (CreateVertexShader(vertexShaderFilePath) == GL_TRUE)
	{
		std::cout<< VertexShaderName <<" created"<<std::endl;		
		if (CreateFragmentShader(fragmentShaderFilePath) == GL_TRUE)
		{
			std::cout << FragmentShaderName <<" created" << std::endl;
			if (CreateProgram() == GL_TRUE)
			{
				bShaderFinalResult = GL_TRUE;
				std::cout << "Shader Program created" << std::endl;
			}
		}
	}		
}

GLboolean Shader::CreateVertexShader(const GLchar* vertexShaderFilePath)
{
	GLboolean result{ GL_FALSE };
	std::ifstream vertexShaderFile;
	vertexShaderFile.exceptions(std::ifstream::badbit);
	try
	{
		vertexShaderFile.open(vertexShaderFilePath);
		std::stringstream vertexShaderStream;
		vertexShaderStream << vertexShaderFile.rdbuf();
		vertexShaderFile.close();
		const std::string vertexCode = vertexShaderStream.str();
		VertexShader = CreateAndCompileShader(GL_VERTEX_SHADER, vertexCode);
		result = GL_TRUE;
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "VertexShaderFile could not be read, "<< e.what() << std::endl;	
		result = GL_FALSE;
		bShaderFinalResult = GL_FALSE;
	}

	return result;
}

GLboolean Shader::CreateFragmentShader(const GLchar* fragmentShaderFilePath)
{
	GLboolean result{ GL_FALSE };
	std::ifstream fragmentShaderFile;
	fragmentShaderFile.exceptions(std::ifstream::badbit);
	try
	{
		fragmentShaderFile.open(fragmentShaderFilePath);
		std::stringstream fragmentShaderStream;
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		fragmentShaderFile.close();
		const std::string fragmentCode = fragmentShaderStream.str();
		FragmentShader = CreateAndCompileShader(GL_FRAGMENT_SHADER, fragmentCode);
		result = GL_TRUE;
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "FragmentShaderFile could not be read, " << e.what() << std::endl;
		result = GL_FALSE;
		bShaderFinalResult = GL_FALSE;
	}

	return result;
}

GLuint Shader::CreateAndCompileShader(const GLenum shaderType, const std::string shaderCode)
{
	GLuint shader = glCreateShader(shaderType);
	const GLchar* shaderCharCode = shaderCode.c_str();
	glShaderSource(shader, 1, &shaderCharCode, 0);
	glCompileShader(shader);
	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		GLchar info[MaxInfoLength];
		glGetShaderInfoLog(shader, MaxInfoLength, 0, info);
		std::cout << info<< std::endl;
		bShaderFinalResult = GL_FALSE;
	}
	return shader;
}

GLboolean Shader::CreateProgram()
{
	Program = glCreateProgram();
	glAttachShader(Program, VertexShader);
	glAttachShader(Program, FragmentShader);
	glLinkProgram(Program);

	GLint linkStatus;
	glGetProgramiv(Program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		GLchar info[MaxInfoLength];
		glGetProgramInfoLog(Program, MaxInfoLength, 0, info);
		std::cout << info<< std::endl;
		bShaderFinalResult = GL_FALSE;
		return GL_FALSE;
	}

	return GL_TRUE;
}

const GLboolean Shader::UseProgram()
{
	if (bShaderFinalResult == GL_TRUE)
	{
		glUseProgram(Program);
	}
	else
	{
		return GL_FALSE;
	}
}

const GLuint Shader::GetUniformLocationFromProgram(const GLchar* uniformName)
{
	return glGetUniformLocation(Program, uniformName);
}