#include <string>

#include "Texture2d.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture2d::Texture2d()
{
	// default constructor should not be used for now
}

// TODO: make a better sytem for handling RBG vs RGBA image loading instead of just a true/false check
Texture2d::Texture2d(std::string filePath, bool flip, bool alpha)
{
	stbi_set_flip_vertically_on_load(flip);

	if (alpha)
	{
		// Load texture
		glGenTextures(1, &_texture_ID);
		glBindTexture(GL_TEXTURE_2D, _texture_ID);

		// Set the default wrapping and filtering options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load and generate texture from given filepath
		unsigned char* data = stbi_load(filePath.c_str(), &_width, &_height, &_num_channels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			std::cout << "RGBA Texture2d created with ID " << _texture_ID << std::endl;
		}
		else
		{
			std::cout << "Failed to load RGBA texture from file: " << filePath << " (in Texture2d constructor)" << std::endl;
		}
	}
	else
	{
		// Load texture
		glGenTextures(1, &_texture_ID);
		glBindTexture(GL_TEXTURE_2D, _texture_ID);

		// Set the default wrapping and filtering options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load and generate texture from given filepath
		unsigned char* data = stbi_load(filePath.c_str(), &_width, &_height, &_num_channels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			std::cout << "RBG Texture2d created with ID " << _texture_ID << std::endl;
		}
		else
		{
			std::cout << "Failed to load RGB texture from file: " << filePath << " (in Texture2d constructor)" << std::endl;
		}
	}

	stbi_image_free(data);
}


Texture2d::~Texture2d()
{
	
}


void Texture2d::bindTexture()
{
	//std::cout << "Binding texture " << _texture_ID << std::endl;
	glBindTexture(GL_TEXTURE_2D, this->_texture_ID);
}

void Texture2d::unbindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture2d::getID() const
{
	return _texture_ID;
}

GLuint Texture2d::getWidth() const
{
	return _width;
}

GLuint Texture2d::getHeight() const
{
	return _height;
}

GLuint Texture2d::getNumChannels() const
{
	return _num_channels;
}