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
	GLuint getProjectionLocation() const;
	GLuint getModelLocation() const;
	GLuint getViewLocation() const;
	GLuint getID() const;
	void setBool(const std::string &name, bool val) const;
	void setInt(const std::string &name, int val) const;
	void setFloat(const std::string &name, float val) const;
	void useShader();
	void clearShader();


protected:
	GLuint _shader_ID, _uniform_projection, _uniform_model, _uniform_view;

	void compile_and_link_shader(const GLchar* vertexCode, const GLchar* fragmentCode);

	//Combined this funtion into compile_and_link_shader function, left for reference/backup
	//void add_shader(GLuint theProgram, const GLchar* shaderCode, GLenum shaderType);
};
#endif // !SHADER_H