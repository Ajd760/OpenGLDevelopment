#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <iostream>

#include <glad\glad.h>

class Texture
{
private:
	GLuint texture_ID;
	int width, height, num_channels;

public:
	unsigned char* data;
	
	Texture();
	Texture(std::string filePath, int* width, int* height, int* nrChannels);
	~Texture();

	GLuint Texture::getID() const;
	GLuint Texture::getWidth() const;
	GLuint Texture::getHeight() const;
	GLuint Texture::getNumChannels() const;
};


#endif // !TEXTURE_H