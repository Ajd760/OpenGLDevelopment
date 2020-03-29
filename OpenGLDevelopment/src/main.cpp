/*
* Include glad before GLFW, otherwise glad will bark about already including opengl headers
*/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main()
{
	std::cout << "Initial commit!" << std::endl;

	return 0;
}