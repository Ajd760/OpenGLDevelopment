#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string>
#include <iostream>

#include <glad\glad.h>

class Texture2d
{
private:
	GLuint texture_ID;
	int width, height, num_channels;

public:
	unsigned char* data;
	
	Texture2d();
	Texture2d(std::string filePath, int* width, int* height, int* nrChannels);
	~Texture2d();

	GLuint Texture2d::getID() const;
	GLuint Texture2d::getWidth() const;
	GLuint Texture2d::getHeight() const;
	GLuint Texture2d::getNumChannels() const;
};


#endif // !TEXTURE2D_H