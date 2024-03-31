#ifndef ShaderInclude
	#define ShaderInclude

#include "GL/glew.h"
#include <string>


class Shader
{
public:	
	Shader(const GLchar* vertexShaderFilePath, const GLchar* fragmentShaderFilePath, const GLchar* VertexShaderName, const GLchar* FragmentShaderName);
	const GLboolean UseProgram();
	const GLuint GetUniformLocationFromProgram(const GLchar* uniformName);

private:
	GLboolean bShaderFinalResult;
	GLuint VertexShader, FragmentShader, Program;

	GLuint CreateAndCompileShader(const GLenum shaderType, const std::string shaderCode);
	GLboolean CreateVertexShader(const GLchar* vertexShaderFilePath);
	GLboolean CreateFragmentShader(const GLchar* fragmentShaderFilePath);
	GLboolean CreateProgram();
};

#endif