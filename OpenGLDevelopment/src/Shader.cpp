#include "Shader.h"

Shader::Shader()
{
	shader_ID = 0;
	uniform_model = 0;
	uniform_projection = 0;
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragPath)
{
	Shader();
	createFromFiles(vertexPath, fragPath);
}

Shader::~Shader()
{
	clearShader();
}


void Shader::createFromString(const GLchar* vertexCode, const GLchar* fragmentCode)
{
	compile_shader(vertexCode, fragmentCode);
}

void Shader::createFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertStream = readFile(vertexLocation);
	std::string fragStream = readFile(fragmentLocation);

	const GLchar* vertCode = vertStream.c_str();
	const GLchar* fragCode = fragStream.c_str();

	compile_shader(vertCode, fragCode);
}

std::string Shader::readFile(const char* fileLocation)
{
	std::string contentRead;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		std::cout << "Failed to open file: " << fileLocation << std::endl;
		return NULL;
	}
	else
	{
		std::string line = "";
		while (!fileStream.eof())
		{
			std::getline(fileStream, line);
			contentRead.append(line + "\n");
		}

		fileStream.close();
		return contentRead;
	}
}

void Shader::compile_shader(const char* vertexCode, const char* fragmentCode)
{
	GLint result = 0;
	GLchar errorLog[1024] = { 0 };

	shader_ID = glCreateProgram();
	if (!shader_ID)
	{
		glGetProgramInfoLog(shader_ID, sizeof(errorLog), NULL, errorLog);
		std::cout << "Error creating shader program: " << errorLog << std::endl;
		return;
	}

	add_shader(shader_ID, vertexCode, GL_VERTEX_SHADER);
	add_shader(shader_ID, fragmentCode, GL_FRAGMENT_SHADER);

	glLinkProgram(shader_ID);
	glGetProgramiv(shader_ID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader_ID, sizeof(errorLog), NULL, errorLog);
		std::cout << "Error linking program: " << errorLog << std::endl;
		return;
	}

	glValidateProgram(shader_ID);
	glGetProgramiv(shader_ID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader_ID, sizeof(errorLog), NULL, errorLog);
		std::cout << "Error validating shader program: " << errorLog << std::endl;
		return;
	}

	uniform_model = glGetUniformLocation(shader_ID, "model");
	uniform_projection = glGetUniformLocation(shader_ID, "projection");
	uniform_view = glGetUniformLocation(shader_ID, "view");
}

void Shader::add_shader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* code[1];
	code[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, code, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar errorLog[1024] = { 0 };
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(errorLog), NULL, errorLog);
		std::cout << "Error compiling a " << shaderType << " shader: " << errorLog << std::endl;
		return;
	}

	glAttachShader(theProgram, theShader);
}

GLuint Shader::getProjectionLocation()
{
	return uniform_projection;
}

GLuint Shader::getModelLocation()
{
	return uniform_model;
}

GLuint Shader::getViewLocation()
{
	return uniform_view;
}

void Shader::useShader()
{
	if (shader_ID == 0)
	{
		std::cout << "Error in Shader::UserShader --> shader_ID == " << shader_ID << std::endl;
	}
	else
	{
		glUseProgram(shader_ID);
	}

}


void Shader::clearShader()
{
	if (shader_ID == 0)
	{
		std::cout << "Error in Shader::ClearShader --> shader_ID == " << shader_ID << ", (tried to clear unallocated shader)" << std::endl;
	}
	else
	{
		glDeleteProgram(shader_ID);
		shader_ID = 0;
	}

	uniform_model = 0;
	uniform_projection = 0;
}