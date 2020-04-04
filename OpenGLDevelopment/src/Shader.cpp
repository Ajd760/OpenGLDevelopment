#include "Shader.h"

Shader::Shader()
{
	shader_ID = 0;
	uniform_model = 0;
	uniform_projection = 0;
}

Shader::~Shader()
{
	ClearShader();
}


void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertStream = ReadFile(vertexLocation);
	std::string fragStream = ReadFile(fragmentLocation);

	const char* vertCode = vertStream.c_str();
	const char* fragCode = fragStream.c_str();

	CompileShader(vertCode, fragCode);
}

std::string Shader::ReadFile(const char* fileLocation)
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

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
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

	AddShader(shader_ID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shader_ID, fragmentCode, GL_FRAGMENT_SHADER);

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

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
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

GLuint Shader::GetProjectionLocation()
{
	return uniform_projection;
}

GLuint Shader::GetModelLocation()
{
	return uniform_model;
}

GLuint Shader::GetViewLocation()
{
	return uniform_view;
}

void Shader::UseShader()
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


void Shader::ClearShader()
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