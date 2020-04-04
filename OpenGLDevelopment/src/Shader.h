#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <iostream>
#include <fstream>

#include <glad\glad.h>

class Shader
{
public:
	Shader();
	Shader(const GLchar* vertexPath, const GLchar* fragPath);
	~Shader();

	void createFromString(const GLchar* vertexCode, const GLchar* fragmentCode);
	void createFromFiles(const char* vertexLocation, const char* fragmentLocation);
	std::string readFile(const char* fileLocation);
	GLuint getProjectionLocation();
	GLuint getModelLocation();
	GLuint getViewLocation();
	void useShader();
	void clearShader();


private:
	GLuint shader_ID, uniform_projection, uniform_model, uniform_view;

	void compile_shader(const char* vertexCode, const char* fragmentCode);
	void add_shader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};
#endif // !SHADER_H