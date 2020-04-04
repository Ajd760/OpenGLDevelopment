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
// This function calls add_shader
void Shader::compile_and_link_shader(const GLchar* vertexCode, const GLchar* fragmentCode)
{
	// error checking variables
	GLint result = 0;
	GLchar errorLog[1024] = { 0 };

	// Create the shader program
	this->shader_ID = glCreateProgram();
	if (!shader_ID)
	{
		glGetProgramInfoLog(shader_ID, sizeof(errorLog), NULL, errorLog);
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
	glAttachShader(shader_ID, vertShader);
	glAttachShader(shader_ID, fragShader);
	
	// Link the shader program
	glLinkProgram(shader_ID);
	glGetProgramiv(shader_ID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader_ID, sizeof(errorLog), NULL, errorLog);
		std::cout << "Error linking program: " << errorLog << std::endl;
		return;
	}

	// Validate the shader program
	glValidateProgram(shader_ID);
	glGetProgramiv(shader_ID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader_ID, sizeof(errorLog), NULL, errorLog);
		std::cout << "Error validating shader program: " << errorLog << std::endl;
		return;
	}

	// Set references to uniform variables
	uniform_model = glGetUniformLocation(shader_ID, "model");
	uniform_projection = glGetUniformLocation(shader_ID, "projection");
	uniform_view = glGetUniformLocation(shader_ID, "view");

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