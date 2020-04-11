#include "Shader.h"

Shader::Shader()
{
	_shader_ID = 0;
	_uniform_model = 0;
	_uniform_projection = 0;
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragPath)
{
	Shader();
	createFromFiles(vertexPath, fragPath);
}

Shader::~Shader()
{

}


void Shader::createFromString(const GLchar* vertexCode, const GLchar* fragmentCode)
{
	compile_and_link_shader(vertexCode, fragmentCode);
}

void Shader::createFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertStream = readFile(vertexLocation);
	std::string fragStream = readFile(fragmentLocation);

	const GLchar* vertCode = vertStream.c_str();
	const GLchar* fragCode = fragStream.c_str();

	compile_and_link_shader(vertCode, fragCode);
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
			std::getline(fileStream, line); //This is probably inefficient? -- TODO: change to sstream reading or something
			contentRead.append(line + "\n");
		}

		fileStream.close();
		return contentRead;
	}
}

// Compiles, links, and validates the shaders given by vertexCode and fragmentCode parameters
void Shader::compile_and_link_shader(const GLchar* vertexCode, const GLchar* fragmentCode)
{
	// error checking variables
	GLint result = 0;
	GLchar errorLog[1024] = { 0 };

	// Create the shader program
	this->_shader_ID = glCreateProgram();
	if (!_shader_ID)
	{
		glGetProgramInfoLog(_shader_ID, sizeof(errorLog), NULL, errorLog);
		std::cout << "Error creating shader program: " << errorLog << std::endl;
		return;
	}

	// Compile vertex shader
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertexCode, NULL);
	glCompileShader(vertShader);
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertShader, sizeof(errorLog), NULL, errorLog);
		std::cout << "Error compiling vertex shader: " << errorLog << std::endl;
		return;
	}

	// Compile fragment shader
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragmentCode, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragShader, sizeof(errorLog), NULL, errorLog);
		std::cout << "Error compiling fragment shader: " << errorLog << std::endl;
		return;
	}

	// Attach both shaders to the shader program
	glAttachShader(_shader_ID, vertShader);
	glAttachShader(_shader_ID, fragShader);
	
	// Link the shader program
	glLinkProgram(_shader_ID);
	glGetProgramiv(_shader_ID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(_shader_ID, sizeof(errorLog), NULL, errorLog);
		std::cout << "Error linking program: " << errorLog << std::endl;
		return;
	}

	// Validate the shader program
	glValidateProgram(_shader_ID);
	glGetProgramiv(_shader_ID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(_shader_ID, sizeof(errorLog), NULL, errorLog);
		std::cout << "Error validating shader program: " << errorLog << std::endl;
		return;
	}

	// Set references to uniform variables
	_uniform_model = glGetUniformLocation(_shader_ID, "model");
	_uniform_projection = glGetUniformLocation(_shader_ID, "projection");
	_uniform_view = glGetUniformLocation(_shader_ID, "view");

	// Can delete the shaders after they have been linked into the program
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}

/*	//Combined this funtion into compile_and_link_shader function, left for reference/backup
// Compile and attach the shader to the current program
void Shader::add_shader(GLuint theProgram, const GLchar* shaderCode, GLenum shaderType)
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
*/

// Uniform getting functions
GLuint Shader::getProjectionLocation() const
{
	return _uniform_projection;
}

GLuint Shader::getModelLocation() const
{
	return _uniform_model;
}

GLuint Shader::getViewLocation() const
{
	return _uniform_view;
}

GLuint Shader::getID() const
{
	return _shader_ID;
}

// Uniform setting functions
void Shader::setBool(const std::string &name, bool val) const
{
	glUniform1i(glGetUniformLocation(this->_shader_ID, name.c_str()), (int)val);
}

void Shader::setInt(const std::string &name, int val) const
{
	glUniform1i(glGetUniformLocation(this->_shader_ID, name.c_str()), val);
}

void Shader::setFloat(const std::string &name, float val) const
{
	glUniform1f(glGetUniformLocation(this->_shader_ID, name.c_str()), val);
}

void Shader::useShader()
{
	if (_shader_ID == 0)
	{
		std::cout << "Error in Shader::UserShader --> shader_ID == " << _shader_ID << std::endl;
	}
	else
	{
		glUseProgram(_shader_ID);
	}

}


void Shader::clearShader()
{
	if (_shader_ID == 0)
	{
		std::cout << "Error in Shader::ClearShader --> shader_ID == " << _shader_ID << ", (tried to clear unallocated shader)" << std::endl;
	}
	else
	{
		glDeleteProgram(_shader_ID);
		std::cout << "Deleted shader program for shader_ID " << _shader_ID << std::endl;
		_shader_ID = 0;
	}

	_uniform_model = 0;
	_uniform_projection = 0;
}