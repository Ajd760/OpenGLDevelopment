/*
* Include glad before GLFW, otherwise glad will bark about already including opengl headers
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Global settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// Ad hoc shaders - temporary
const char* vertShaderSrc = "#version 330 core\n"
						"layout (location = 0) in vec3 aPos;\n"
						"void main()\n"
						"{\n"
						"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
						"}\n\0";
const char* fragShaderSrc = "#version 330 core\n"
						"out vec4 FragColor;\n"
						"void main()\n"
						"{\n"
						"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
						"}\n\0";


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


	// Compile and Link Shaders
	// ----------------------------------------------------
	// Compile vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertexShader);

	// Check vertex shader compilation
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED!\n" << infoLog << std::endl;
	}

	// Compile fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragmentShader);

	// Check fragment shader compilation
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED!\n" << infoLog << std::endl;
	}

	//Link Shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check shader program linking for errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED!\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	// ----------------------------------------------------


	// Create vertex and buffer data, configure vertex attributes
	// ----------------------------------------------------
	// Create triangle
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	// Create and bind a Vertex Buffer Object and Vertex Array Object, and give them the data in the vertices[] array
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object first, then bind/set the Vertex buffer object
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// glVertexAttribPointer tells openGL how to process the vertex array data (starting at psition 0 in the array, stride of 3, data values are floats, etc)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //This last param is the offset of where the position data begins in the buffer, and requires the cast (void*)
	glEnableVertexAttribArray(0);

	// Unbind the VBO since the call to glVertexAttribPointer registered the VBO as the vertex attribute's bound VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// ** Can unbind the VAO so that other VAO calls won't accidentally modify this VAO, however this rarely happens since 
	//		modifying other VAOs require a call to glBindVertexArray anyways
	glBindVertexArray(0);


	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Check inputs
		processInput(window);

		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	//Clear screen with a grey/green color
		glClear(GL_COLOR_BUFFER_BIT);			// Actually clear the screen

		// Draw a triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);				// Not really necessary to bind every loop since we only have a single VAO right now
		glDrawArrays(GL_TRIANGLES, 0, 3);	//Drawing a triangle, 
		// glBindVertexArray(0);  //don't need to unbind every time

		// Check/call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Deallocate everything before program end
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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