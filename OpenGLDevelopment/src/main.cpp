/// Next TODO:
// Implement Texture wrapper class
// Implement GLFW Window wrapper class

// Page 64

/*
* Include glad before GLFW, otherwise glad will bark about already including opengl headers
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"




void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Global settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;


int main()
{
	///Init stuff
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGLDevelopment", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create a GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Set callback function to be called each time window is resized
	/// End Init stuff


	// Create a shader using the new shader class ------------------------------------------
	Shader shader("shaders/shader.vert", "shaders/shader.frag");
	std::cout << "Shader created with ID " << shader.getID() << std::endl;
	// -------------------------------------------------------------------------------------

	// Create vertex and buffer data, configure vertex attributes
	// -----------------------------------------------------------
	// Create a box, with 3 types of attributes - position, color, and texture coords
	float vertices[] = {
		//positions		     // colors
		 0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, //top right
		 0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, //bottom right
		-0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, //bottom left
		-0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f //top left
	};
	
	// Indices to create 2 triangles if using an EBO
	unsigned int indices[] = {
		0, 1, 3, //first triangle
		1, 2, 3  //second triangle
	};

	// Create and bind a Vertex Buffer Object and Vertex Array Object, and give them the data in the vertices[] array
	// Also create an Element Buffer Object to hold
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object first, then bind/set the Vertex buffer object, then the element buffer object
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// glVertexAttribPointer tells openGL how to process the vertex array data (starting at position 0 in the array, attributes have 3 values (vec3), data values are floats, stride of 3*varialble size, etc)
	// The position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //This last param is the offset of where the position data begins in the buffer, and requires the cast (void*)
	glEnableVertexAttribArray(0);

	// The color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //This last param is the offset of where the color data begins in the buffer, and requires the cast (void*)
	glEnableVertexAttribArray(1);

	// The texture attribute
	


	// Unbind the VBO since the call to glVertexAttribPointer registered the VBO as the vertex attribute's bound VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// ** Can unbind the VAO so that other VAO calls won't accidentally modify this VAO, however this rarely happens since 
	//		modifying other VAOs require a call to glBindVertexArray anyways
	glBindVertexArray(0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment to draw in WIREFRAME mode
	// -----------------------------------------------------------

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Check inputs
		processInput(window);

		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	//Clear screen with a grey/green color
		glClear(GL_COLOR_BUFFER_BIT);			// Actually clear the screen
		shader.useShader();

		// Create a color change from red to black and back to red based on time
		//float timeVal = glfwGetTime();
		//float redValue = sin(timeVal) / 2.0f + 0.5f;
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//glUniform4f(vertexColorLocation, redValue, 0.0f, 0.0f, 1.0f);

		//2. Set a horizontal offset via a uniform that we add to the vertex shader
		float xOffset = 0.0f; //set triangle back to center but left this offset in
		shader.setFloat("xOffset", xOffset);

		// Draw a triangle
		glBindVertexArray(VAO);				// Not really necessary to bind every loop since we only have a single VAO right now
		//glDrawArrays(GL_TRIANGLES, 0, 3);	//Drawing a triangle, starting at index 0 of the bound array, drawing 3 vertices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //Drawing 2 triangles to form a rectangle with an EBO
		// glBindVertexArray(0);  //don't need to unbind every time

		// Check/call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Deallocate everything before program end
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	shader.clearShader();
	glfwTerminate();
	return 0;
}


// Callback function to be registered to the window that should get called 
//	each time the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Processes user input each frame
void processInput(GLFWwindow* window)
{
	// Check if user pressed escape, if so close the window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}